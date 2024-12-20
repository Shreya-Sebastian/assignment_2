#include "mesh.h"
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <tinyobjloader/tiny_obj_loader.h>
DISABLE_WARNINGS_POP()
#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <numeric>
#include <span>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>

static void centerAndScaleToUnitMesh(std::span<Mesh> meshes);

static glm::vec3 construct_vec3(const float* pFloats)
{
    return glm::vec3(pFloats[0], pFloats[1], pFloats[2]);
}

// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
template <class T>
static void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct VertexHash {
    size_t operator()(const Vertex& v) const
    {
        size_t seed = 0;
        hash_combine(seed, v.position.x);
        hash_combine(seed, v.position.y);
        hash_combine(seed, v.position.z);
        hash_combine(seed, v.normal.x);
        hash_combine(seed, v.normal.y);
        hash_combine(seed, v.normal.z);
        hash_combine(seed, v.texCoord.s);
        hash_combine(seed, v.texCoord.t);
        return seed;
    }
};

std::vector<Mesh> loadMesh(const std::filesystem::path& file, bool centerAndNormalize)
{
    if (!std::filesystem::exists(file)) {
        std::cerr << "File " << file << " does not exist." << std::endl;
        throw std::exception();
    }

    const auto baseDir = file.parent_path();

    tinyobj::attrib_t inAttrib;
    std::vector<tinyobj::shape_t> inShapes;
    std::vector<tinyobj::material_t> inMaterials;

    std::string warn, error;
    bool ret = tinyobj::LoadObj(&inAttrib, &inShapes, &inMaterials, &warn, &error, file.string().c_str(), baseDir.string().c_str());
    if (!ret) {
        std::cerr << "Failed to load mesh " << file << std::endl;
        throw std::exception();
    }

    std::vector<Mesh> out;
    for (const auto& shape : inShapes) {
        assert(shape.mesh.indices.size() % 3 == 0);

        size_t startTriangle = 0;
        auto prevMaterialID = shape.mesh.material_ids[0];
        for (size_t endTriangle = 0; endTriangle < shape.mesh.indices.size() / 3; ++endTriangle) {
            // tinyobjloader does not automatically split the mesh into smaller sub meshes according to material so we have to do it ourselves.
            if (endTriangle == shape.mesh.indices.size() / 3 - 1)
                ++endTriangle; // End of the tinyobj.shape; write remaining mesh.
            else if (shape.mesh.material_ids[endTriangle] == prevMaterialID)
                continue;
            else
                prevMaterialID = shape.mesh.material_ids[endTriangle];

            Mesh mesh;
            std::unordered_map<Vertex, uint32_t, VertexHash> vertexCache; // Map the index of a vertex as loaded by tinyobjloader to its index in the generated mesh
            for (size_t i = startTriangle * 3; i != endTriangle * 3; i += 3) {
                const glm::vec3 v0 = construct_vec3(&inAttrib.vertices[3 * shape.mesh.indices[i + 0].vertex_index]);
                const glm::vec3 v1 = construct_vec3(&inAttrib.vertices[3 * shape.mesh.indices[i + 1].vertex_index]);
                const glm::vec3 v2 = construct_vec3(&inAttrib.vertices[3 * shape.mesh.indices[i + 2].vertex_index]);
                const auto geometricNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

                // Load the triangle indices and lazily create the vertices.
                glm::uvec3 triangle;
                for (unsigned j = 0; j < 3; j++) {
                    const auto& tinyObjIndex = shape.mesh.indices[i + j];
                    Vertex vertex {
                        .position = construct_vec3(&inAttrib.vertices[3 * tinyObjIndex.vertex_index]),
                        .normal = glm::vec3(0),
                        .texCoord = glm::vec2(0)
                    };
                    if (tinyObjIndex.normal_index != -1 && !inAttrib.normals.empty())
                        vertex.normal = glm::vec3(inAttrib.normals[3 * tinyObjIndex.normal_index + 0], inAttrib.normals[3 * tinyObjIndex.normal_index + 1], inAttrib.normals[3 * tinyObjIndex.normal_index + 2]);
                    else
                        vertex.normal = geometricNormal;
                    if (tinyObjIndex.texcoord_index != -1 && !inAttrib.texcoords.empty())
                        vertex.texCoord = glm::vec2(inAttrib.texcoords[2 * tinyObjIndex.texcoord_index + 0], inAttrib.texcoords[2 * tinyObjIndex.texcoord_index + 1]);

                    if (auto iter = vertexCache.find(vertex); iter != std::end(vertexCache)) {
                        // Already visited this vertex? Reuse it!
                        triangle[j] = iter->second;
                    } else {                      
                        // New vertex? Create it and store it in the vertex cache.
                        vertexCache[vertex] = triangle[j] = (unsigned)mesh.vertices.size();
                        mesh.vertices.push_back(vertex);
                    }
                }
                mesh.triangles.push_back(triangle);
            }

            const auto materialID = shape.mesh.material_ids[startTriangle];
            if (materialID == -1) {
                mesh.material.kd = glm::vec3(1.0f);
                mesh.material.ks = glm::vec3(0.0f);
                mesh.material.shininess = 1.0f;
            } else {
                const auto& objMaterial = inMaterials[materialID];
                mesh.material.kd = construct_vec3(objMaterial.diffuse);
                if (!objMaterial.diffuse_texname.empty()) {
                    mesh.material.kdTexture = std::make_shared<Image>(baseDir / objMaterial.diffuse_texname);
                }
                mesh.material.ks = construct_vec3(objMaterial.specular);
                mesh.material.shininess = objMaterial.shininess;
                mesh.material.transparency = objMaterial.dissolve;
            }

            out.push_back(std::move(mesh));

            startTriangle = endTriangle;
        }
    }

    if (centerAndNormalize)
        centerAndScaleToUnitMesh(out);

    return out;
}

static void centerAndScaleToUnitMesh(std::span<Mesh> meshes)
{
    std::vector<glm::vec3> positions;
    for (const auto& mesh : meshes)
        std::transform(std::begin(mesh.vertices), std::end(mesh.vertices),
            std::back_inserter(positions),
            [](const Vertex& v) { return v.position; });
    const glm::vec3 center = std::accumulate(std::begin(positions), std::end(positions), glm::vec3(0.0f)) / static_cast<float>(positions.size());
    float maxD = 0.0f;
    for (const glm::vec3& p : positions)
        maxD = std::max(glm::length(p - center), maxD);
    /*// REQUIRES A MODERN COMPILER
      const float maxD = std::transform_reduce(
              std::begin(vertices), std::end(vertices),
              0.0f,
              [](float lhs, float rhs) { return std::max(lhs, rhs); },
              [=](const Vertex& v) { return glm::length(v.pos - center); });*/

    for (auto& mesh : meshes) {
        std::transform(std::begin(mesh.vertices), std::end(mesh.vertices),
            std::begin(mesh.vertices), [=](Vertex v) {
                v.position = (v.position - center) / maxD;
                return v;
            });
    }
}

Mesh mergeMeshes(std::span<const Mesh> meshes)
{
    Mesh out;
    out.material = meshes[0].material;
    for (const auto& mesh : meshes) {
        const auto vertexOffset = out.vertices.size();
        out.vertices.resize(out.vertices.size() + mesh.vertices.size());
        std::copy(std::begin(mesh.vertices), std::end(mesh.vertices), std::begin(out.vertices) + vertexOffset);

        for (const auto& tri : mesh.triangles) {
            out.triangles.push_back(tri + (unsigned)vertexOffset);
        }
    }
    return out;
}

void  meshFlipX(Mesh& mesh)
{
    for (auto& v : mesh.vertices) {
        v.position.x = -v.position.x;
        v.normal.x = -v.normal.x;
    }
}

void  meshFlipY(Mesh& mesh)
{
    for (auto& v : mesh.vertices) {
        v.position.y = -v.position.y;
        v.normal.y = -v.normal.y;
    }
}

void meshFlipZ(Mesh& mesh)
{
    for (auto& v : mesh.vertices) {
        v.position.z = -v.position.z;
        v.normal.z = -v.normal.z;
    }
}

glm::vec3 safeNormalize(const glm::vec3& v)
{
    float len = glm::length(v);
    return len > 0.0f ? v / len : glm::vec3(0.0f);
}

void calculateTangents( Mesh& mesh)
{
    // Initialize all tangents to zero
    for (auto& vertex : mesh.vertices) {
        vertex.tangent = glm::vec4(0.0f);
    }

    // Loop through each triangle
    for (size_t i = 0; i < mesh.triangles.size(); i += 3) {
        // Get the indices for the triangle's vertices
        unsigned int i0 = mesh.triangles[i].x;
        unsigned int i1 = mesh.triangles[i].y;
        unsigned int i2 = mesh.triangles[i].z;

        // Get the vertices of the triangle
        Vertex& v0 = mesh.vertices[i0];
        Vertex& v1 = mesh.vertices[i1];
        Vertex& v2 = mesh.vertices[i2];

        // Calculate the edges of the triangle
        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        // Calculate the difference in UV coordinates
        glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
        glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

        // Check for degenerate UV coordinates
        float determinant = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
        if (determinant == 0.0f) {
            continue; // Skip this triangle if UVs are degenerate
        }
        float f = 1.0f / determinant;

        // Calculate the tangent
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        tangent = safeNormalize(tangent);

        // Calculate the handedness (bitangent direction) using cross product
        glm::vec3 normal = safeNormalize(v0.normal);
        float handedness = (glm::dot(glm::cross(normal, tangent), edge2) < 0.0f) ? -1.0f : 1.0f;

        // Accumulate the tangents into each vertex of the triangle
        v0.tangent += glm::vec4(tangent, handedness);
        v1.tangent += glm::vec4(tangent, handedness);
        v2.tangent += glm::vec4(tangent, handedness);
    }

    // Normalize the tangents for each vertex
    for (auto& vertex : mesh.vertices) {
        vertex.tangent = glm::vec4(safeNormalize(glm::vec3(vertex.tangent)), vertex.tangent.w);
        printf("%f, %f, %f\n", vertex.tangent.x, vertex.tangent.y, vertex.tangent.z);
    }
}
