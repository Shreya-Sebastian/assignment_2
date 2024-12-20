//#include "Image.h"
#include "mesh.h"
#include "framework/mesh.h"
#include "texture.h"
#include "camera.h"
// Always include window first (because it includes glfw, which includes GL which needs to be included AFTER glew).
// Can't wait for modules to fix this stuff...
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glad/glad.h>
// Include glad before glfw3
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <imgui/imgui.h>
DISABLE_WARNINGS_POP()
#include <framework/shader.h>
#include <framework/window.h>
#include <framework/image.h>
#include <functional>
#include <iostream>
#include <vector>
#include "tiny_obj_loader.h"
#include <glm/gtx/component_wise.hpp>
#include <random>



Mesh createCubeMesh() {
    Mesh cubeMesh;

    // Define vertices for a cube with normals and texture coordinates.
    cubeMesh.vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Top-left

        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}}, // Top-left

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left
    };

    // Define triangles using indices to the vertices above
    cubeMesh.triangles = {
        // Front face
        {0, 1, 2}, {0, 2, 3},
        // Back face
        {4, 5, 6}, {4, 6, 7},
        // Left face
        {8, 9, 10}, {8, 10, 11},
        // Right face
        {12, 13, 14}, {12, 14, 15},
        // Top face
        {16, 17, 18}, {16, 18, 19},
        // Bottom face
        {20, 21, 22}, {20, 22, 23}
    };

    return cubeMesh;
}

float rectangleVertices[] =
{
    // Coords    // texCoords
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};

float skyboxVertices[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,  // 0: Top-left-front
    -1.0f, -1.0f, -1.0f,  // 1: Bottom-left-front
     1.0f, -1.0f, -1.0f,  // 2: Bottom-right-front
     1.0f,  1.0f, -1.0f,  // 3: Top-right-front
    -1.0f,  1.0f,  1.0f,  // 4: Top-left-back
    -1.0f, -1.0f,  1.0f,  // 5: Bottom-left-back
     1.0f, -1.0f,  1.0f,  // 6: Bottom-right-back
     1.0f,  1.0f,  1.0f   // 7: Top-right-back
};

unsigned int skyboxIndices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Back face
    4, 5, 6,
    6, 7, 4,
    // Left face
    4, 5, 1,
    1, 0, 4,
    // Right face
    3, 2, 6,
    6, 7, 3,
    // Top face
    4, 0, 3,
    3, 7, 4,
    // Bottom face
    1, 5, 6,
    6, 2, 1
};


unsigned int quadVAO = 0;
unsigned int quadVBO;

void createPlane() {

    // positions
    glm::vec3 pos1(-1.0, 1.0, 0.0);
    glm::vec3 pos2(-1.0, -1.0, 0.0);
    glm::vec3 pos3(1.0, -1.0, 0.0);
    glm::vec3 pos4(1.0, 1.0, 0.0);
    // texture coordinates
    glm::vec2 uv1(0.0, 1.0);
    glm::vec2 uv2(0.0, 0.0);
    glm::vec2 uv3(1.0, 0.0);
    glm::vec2 uv4(1.0, 1.0);
    // normal vector
    glm::vec3 nm(0.0, 0.0, 1.0);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);



    float quadVertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };

    // configure plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }

struct AABB {
    float x_min, x_max;
    float y_min, y_max;
};


class Application {
public:
    Application()
        : m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL41)
        , m_texture(RESOURCE_ROOT "resources/textures/wall.jpeg")
        , m_normalMap(RESOURCE_ROOT "resources/textures/fur_normalmap.jpg")
    {
        m_window.registerKeyCallback([this](int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS)
                onKeyPressed(key, mods);
            else if (action == GLFW_RELEASE)
                onKeyReleased(key, mods);
            });
        m_window.registerMouseMoveCallback(std::bind(&Application::onMouseMove, this, std::placeholders::_1));
        m_window.registerMouseButtonCallback([this](int button, int action, int mods) {
            if (action == GLFW_PRESS)
                onMouseClicked(button, mods);
            else if (action == GLFW_RELEASE)
                onMouseReleased(button, mods);
            });

        GPUMesh cubeMesh1 = GPUMesh(createCubeMesh());  // First cube mesh
        GPUMesh cubeMesh2 = GPUMesh(createCubeMesh());  // Second cube mesh
        wolfMeshes = GPUMesh::loadMeshGPU(RESOURCE_ROOT "resources/wolf/Wolf_obj.obj");

        m_meshes.emplace_back(createCubeMesh());  // Construct first cube mesh in-place
        m_meshes.emplace_back(createCubeMesh());


        // Defining the faces of the skybox
        std::vector<std::string> faces = {
            "resources/px.png", "resources/nx.png", "resources/py.png", "resources/ny.png", "resources/pz.png", "resources/nz.png"
        };
        unsigned int cubemapTexture = loadCubemap(faces); // Function to load cubemap texture

        // Defining the bezier curves
        arcLengths.resize(NUM_SAMPLES);
        bezierPoints.resize(NUM_SAMPLES);
        std::vector<glm::vec3> controlPoints = {
            startPoint1, controlPoint1, controlPoint2, endPoint1, // First curve
            startPoint2, controlPoint3, controlPoint4, endPoint2, // Second curve
            startPoint3, controlPoint5, controlPoint6, endPoint3, // Third curve
            startPoint4, controlPoint7, controlPoint8, endPoint4  // Fourth curve
        };
        generateArcLengthTable(controlPoints);

        try {


            ShaderBuilder defaultBuilder;
            defaultBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/shader_vert.glsl");
            defaultBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/shader_frag.glsl");
            m_defaultShader = defaultBuilder.build();

            ShaderBuilder shadowBuilder;
            shadowBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/shadow_vert.glsl");
            shadowBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "Shaders/shadow_frag.glsl");
            m_shadowShader = shadowBuilder.build();

            // Load shaders
            ShaderBuilder reflectionBuilder;
            reflectionBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/reflection_vert.glsl");
            reflectionBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/reflection_frag.glsl");
            m_reflectionShader = reflectionBuilder.build();

            ShaderBuilder skyboxBuilder;
            skyboxBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/skybox_vert.glsl");
            skyboxBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "Shaders/skybox_frag.glsl");
            m_skyboxShader = skyboxBuilder.build();

            ShaderBuilder normalBuilder;
            normalBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/normal_vert.glsl");
            normalBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/normal_frag.glsl");
            m_normalShader = normalBuilder.build();

            ShaderBuilder fboBuilder;
            fboBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/framebuffer_vert.glsl");
            fboBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/framebuffer_frag.glsl");
            m_fboShader = fboBuilder.build();

            ShaderBuilder bloomBuilder;
            bloomBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/framebuffer_vert.glsl");
            bloomBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/bloom_frag.glsl");
            m_bloomShader = bloomBuilder.build();

            // Any new shaders can be added below in similar fashion.
            // ==> Don't forget to reconfigure CMake when you do!
            //     Visual Studio: PROJECT => Generate Cache for ComputerGraphics
            //     VS Code: ctrl + shift + p => CMake: Configure => enter
            // ....
        }
        catch (ShaderLoadingException e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void update()
    {
        float angle1 = 0.0f;
        float angle2 = 0.0f;
        float parentYPosition = 0.0f;
        float direction = 1.0f;
        int collisionCounter = 0; // Initialized to 0
        glm::vec3 childColor(0.0, 0.4, 1.0);
        glm::vec3 parentColor(1.0, 1.0, 0.0);
        m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, glm::vec3(2.5f, 2.0f, 2.5f));

        // Set up the skybox VAO, VBO, and EBO
        unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        unsigned int rectVAO, rectVBO;
        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Create Frame Buffer Object
        unsigned int FBO;
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create Framebuffer Texture
        unsigned int framebufferTexture;
        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        // Create Render Buffer Object
        unsigned int RBO;
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


        unsigned int bloomTexture;
        glGenTextures(1, &bloomTexture);
        glBindTexture(GL_TEXTURE_2D, bloomTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

  
        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);


        unsigned int pingpongFBO[2];
        unsigned int pingpongBuffer[2];
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongBuffer);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
        }

        GLuint wolfVBO, wolfVAO, wolfNBO, wolfTBO;
        glGenVertexArrays(1, &wolfVAO);
        glGenBuffers(1, &wolfVBO);
        glGenBuffers(1, &wolfNBO);
        glGenBuffers(1, &wolfTBO);

        // Bind VAO
        glBindVertexArray(wolfVAO);

        // Vertex positions
        glBindBuffer(GL_ARRAY_BUFFER, wolfVBO);
        glBufferData(GL_ARRAY_BUFFER, wolfVertices.size() * sizeof(glm::vec3), wolfVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glBindBuffer(GL_ARRAY_BUFFER, wolfNBO);
        glBufferData(GL_ARRAY_BUFFER, wolfNormals.size() * sizeof(glm::vec3), wolfNormals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        // Texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, wolfTBO);
        glBufferData(GL_ARRAY_BUFFER, wolfTexCoords.size() * sizeof(glm::vec2), wolfTexCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);

        // Unbind VAO
        glBindVertexArray(0);

        std::vector<glm::vec3> wolfTangents(wolfVertices.size());
        std::vector<glm::vec3> wolfBitangents(wolfVertices.size());

        for (size_t i = 0; i < wolfVertices.size(); i += 3) {
            glm::vec3 pos1 = wolfVertices[i];
            glm::vec3 pos2 = wolfVertices[i + 1];
            glm::vec3 pos3 = wolfVertices[i + 2];

            glm::vec2 uv1 = wolfTexCoords[i];
            glm::vec2 uv2 = wolfTexCoords[i + 1];
            glm::vec2 uv3 = wolfTexCoords[i + 2];

            glm::vec3 edge1 = pos2 - pos1;
            glm::vec3 edge2 = pos3 - pos1;
            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent;
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            glm::vec3 bitangent;
            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);

            wolfTangents[i] = tangent;
            wolfTangents[i + 1] = tangent;
            wolfTangents[i + 2] = tangent;

            wolfBitangents[i] = bitangent;
            wolfBitangents[i + 1] = bitangent;
            wolfBitangents[i + 2] = bitangent;
        }


        GLuint wolfTanBO, wolfBitanBO;
        glGenBuffers(1, &wolfTanBO);
        glGenBuffers(1, &wolfBitanBO);

        // Tangents
        glBindBuffer(GL_ARRAY_BUFFER, wolfTanBO);
        glBufferData(GL_ARRAY_BUFFER, wolfTangents.size() * sizeof(glm::vec3), wolfTangents.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(3);

        // Bitangents
        glBindBuffer(GL_ARRAY_BUFFER, wolfBitanBO);
        glBufferData(GL_ARRAY_BUFFER, wolfBitangents.size() * sizeof(glm::vec3), wolfBitangents.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(4);

        glBindVertexArray(0);

        float lastFrameTime = static_cast<float>(glfwGetTime());
        const float positionTolerance = 0.01f;  // Tolerance for reaching the ends

        glm::vec3 bezierPosition;
        glm::vec3 randomPosition = glm::vec3(0.0f);
        // Seed random generator
        std::mt19937 rng(static_cast<unsigned int>(glfwGetTime()));
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        // Initialize position and direction variables if not already done
        float cubeXPosition = 0.0f; // Cube’s X position
        float cubeYPosition = 0.0f; // Cube’s Y position
        float xDirection = 1.0f;    // X movement direction
        float yDirection = 1.0f;    // Y movement direction

        // Define movement boundaries and speed
        const float xBoundary = 1.0f;   // X axis boundary (adjust to room size)
        const float yBoundary = 1.0f;   // Y axis boundary (adjust to room size)
        const float movementSpeed = 0.005f; // Speed of movement



        while (!m_window.shouldClose()) {
            printf("%i", followedModel);

            // Time logic for the bezier curve moves
            float currentFrameTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            time += speed * deltaTime;  // deltaTime is the frame time
            time = glm::mod(time, 1.0f);  // Keep time within [0, 1]

            // Time logic for the multiple texture loading
            m_timeAccumulator += deltaTime;
            /*if (m_timeAccumulator >= m_animationSpeed) {
                m_timeAccumulator = 0.0f;
                m_currentFrame = (m_currentFrame + 1) % m_animationTextures.size();
            }*/


            //glm::vec3 bezierPosition = getConstantSpeedPosition(time);

            m_window.updateInput();
            angle1 += 0.001f;
            angle2 += 0.0025f;
            // ImGui controls
            ImGui::Begin("Window");
            ImGui::InputInt("This is the collision counter", &collisionCounter);
            ImGui::Text("Value is: %i", collisionCounter);
            ImGui::Checkbox("Use material if no texture", &m_useMaterial);
            const char* options[] = { "Free camera", "Wolf 1", "Wolf 2" };

            if (ImGui::BeginCombo("Choose Followed Model", options[followedModel])) {
                if (ImGui::Selectable("Free camera", followedModel == 0)) {
                    followedModel = 0;
                    camera.setFree();
                }
                if (ImGui::Selectable("Wolf 1", followedModel == 1)) {
                    followedModel = 1;
                    camera.setFollowCharacter(m_modelMatrix_wolf);
                }
                if (ImGui::Selectable("Wolf 2", followedModel == 2)) {
                    followedModel = 2;
                    camera.setFollowCharacter(m_modelMatrix_wolf_2);
                }
                m_viewMatrix = camera.viewMatrix();

                ImGui::EndCombo();
            }
            if (ImGui::Button("Top view")) {
                camera.setTopView();
                m_viewMatrix = camera.viewMatrix();
            }
            ImGui::Text("Post processing options");
            ImGui::Checkbox("Inverse color", &inverseColor);
            ImGui::Checkbox("Black and white", &blacnAndWhite);
            ImGui::Checkbox("Bloom", &bloom);

            ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
            ImGui::ColorEdit3("Wolf Color", glm::value_ptr(parentColor));
            ImGui::Checkbox("Normal Mapping", &m_normalMapping);
            ImGui::Checkbox("Wolf 1 PBR", &wolf1pbr);
            ImGui::Checkbox("Wolf 2 PBR", &wolf2pbr);
            ImGui::Text("PBR settings");
            ImGui::SliderFloat("Wolf 1 metallic", &wolf1metallic, 0.0f, 1.0f);
            ImGui::SliderFloat("Wolf 1 roughness", &wolf1roughness, 0.0f, 1.0f);
            ImGui::SliderFloat("Wolf 2 metallic", &wolf2metallic, 0.0f, 1.0f);
            ImGui::SliderFloat("Wolf 2 roughness", &wolf2roughness, 0.0f, 1.0f);
            ImGui::Text("Simple shader settings");
            ImGui::ColorEdit3("Ks", glm::value_ptr(ks));
            ImGui::ColorEdit3("Kd", glm::value_ptr(kd));
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
            ImGui::Checkbox("Specular", &specular);
            ImGui::Checkbox("Bezier Curve Movement", &m_bezierMovementEnabled);
            ImGui::Checkbox("Enable Environment Mapping", &m_envMapEnabled);


            ImGui::End();


            m_timer += 0.1f;
            glm::mat4 Modelmat(1.f);
            m_modelMatrix = glm::rotate(Modelmat, glm::radians(m_timer), glm::vec3(1.f, 1.f, 0.f));


            // Clear the screen
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            //glDisable(GL_BLEND);

            
            m_normalShader.bind();
            glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);

            
            for (GPUMesh& mesh : wolfMeshes) {
                const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix_wolf;
                glUniformMatrix4fv(m_normalShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
                const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix_wolf));
                glUniformMatrix3fv(m_normalShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
                glUniform3fv(m_normalShader.getUniformLocation("color"), 1, glm::value_ptr(parentColor));
                glUniform3fv(m_normalShader.getUniformLocation("lightPos"), 1, glm::value_ptr(lightPos));
                glUniform3fv(m_normalShader.getUniformLocation("lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(m_normalShader.getUniformLocation("kd"), 1, glm::value_ptr(kd));
                glUniform3fv(m_normalShader.getUniformLocation("ks"), 1, glm::value_ptr(ks));
                glUniform3fv(m_normalShader.getUniformLocation("cameraPos"), 1, glm::value_ptr(camera.cameraPos()));
                glUniform1i(m_normalShader.getUniformLocation("wolf"), wolf1pbr);
                glUniform1f(m_normalShader.getUniformLocation("metallic"), wolf1metallic);
                glUniform1f(m_normalShader.getUniformLocation("roughness"), wolf1roughness);
                glUniform1f(m_normalShader.getUniformLocation("shininess"), shininess);
                m_normalMap.bind(GL_TEXTURE1);
                glUniform1i(m_normalShader.getUniformLocation("normalMap"), 1);
                glUniform1i(m_normalShader.getUniformLocation("normalMapping"), m_normalMapping);
                mesh.draw(m_normalShader);
            }

            m_normalShader.bind();
            for (GPUMesh& mesh : wolfMeshes) {
                const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix_wolf_2;
                glUniformMatrix4fv(m_normalShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
                const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix_wolf_2));
                glUniformMatrix3fv(m_normalShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
                glUniform3fv(m_normalShader.getUniformLocation("color"), 1, glm::value_ptr(parentColor));
                glUniform3fv(m_normalShader.getUniformLocation("lightPos"), 1, glm::value_ptr(lightPos));
                glUniform3fv(m_normalShader.getUniformLocation("lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(m_normalShader.getUniformLocation("kd"), 1, glm::value_ptr(kd));
                glUniform3fv(m_normalShader.getUniformLocation("ks"), 1, glm::value_ptr(ks));
                glUniform3fv(m_normalShader.getUniformLocation("cameraPos"), 1, glm::value_ptr(camera.cameraPos()));
                glUniform1i(m_normalShader.getUniformLocation("wolf"), wolf2pbr);
                glUniform1f(m_normalShader.getUniformLocation("metallic"), wolf2metallic);
                glUniform1f(m_normalShader.getUniformLocation("roughness"), wolf2roughness);
                glUniform1f(m_normalShader.getUniformLocation("shininess"), shininess);
                glUniform1i(m_normalShader.getUniformLocation("specular"), specular);
                m_normalMap.bind(GL_TEXTURE1);
                glUniform1i(m_normalShader.getUniformLocation("normalMap"), 1);
                glUniform1i(m_normalShader.getUniformLocation("normalMapping"), m_normalMapping);
                mesh.draw(m_normalShader);
            }

            glm::vec3 cubePosition;
            // Toggle movement mode
            if (m_bezierMovementEnabled) {
                bezierPosition = getConstantSpeedPosition(time);
                cubePosition = bezierPosition;
            }
            else {
                // Move the cube left/right in the X axis
                cubeXPosition += movementSpeed * xDirection;
                if (cubeXPosition > xBoundary || cubeXPosition < -xBoundary) {
                    xDirection *= -1.0f; 
                }

                // Move the cube up/down in the Y axis
                cubeYPosition += movementSpeed * yDirection;
                if (cubeYPosition > yBoundary || cubeYPosition < -yBoundary) {
                    yDirection *= -1.0f; 
                }

                // Set the cube position, keeping Z position unchanged
                cubePosition = glm::vec3(cubeXPosition, cubeYPosition, 0.0f);
            }

            glm::mat4 modelMatrixParent = glm::translate(glm::mat4(1.0f), cubePosition);
            modelMatrixParent = glm::scale(modelMatrixParent, glm::vec3(0.2f));
            modelMatrixParent = glm::rotate(modelMatrixParent, angle1, glm::vec3(0.0f, 1.0f, 0.0f));

            // Switch to the reflection shader for the reflective cube
            /*m_reflectionShader.bind();


            // // Parent Cube Transformation (environment-mapped reflective cube)
            glUniformMatrix4fv(m_reflectionShader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrixParent));
            glUniformMatrix4fv(m_reflectionShader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
            glUniformMatrix4fv(m_reflectionShader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

            // Set the camera position for accurate reflection direction calculation
            glUniform3fv(m_reflectionShader.getUniformLocation("cameraPosition"), 1, glm::value_ptr(cameraPos));

            // Bind the cubemap texture to texture unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glUniform1i(m_reflectionShader.getUniformLocation("skybox"), 0); // Set the cubemap sampler uniform

            // Draw the parent (larger) cube with reflection
            m_meshes[0].draw(m_reflectionShader);*/

            if (m_envMapEnabled) {
                // Use reflection shader for environment mapping
                m_reflectionShader.bind();
                glUniformMatrix4fv(m_reflectionShader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrixParent));
                glUniformMatrix4fv(m_reflectionShader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
                glUniformMatrix4fv(m_reflectionShader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
                glUniform3fv(m_reflectionShader.getUniformLocation("cameraPosition"), 1, glm::value_ptr(cameraPos));

                // Bind the cubemap texture for environment mapping
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                glUniform1i(m_reflectionShader.getUniformLocation("skybox"), 0);
            }
            else {
                // Use default shader
                m_defaultShader.bind();
                glUniformMatrix4fv(m_defaultShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix * m_viewMatrix * modelMatrixParent));
                glUniform3fv(m_defaultShader.getUniformLocation("color"), 1, glm::value_ptr(parentColor));
            }

            // Draw the larger cube
            m_meshes[0].draw(m_envMapEnabled ? m_reflectionShader : m_defaultShader);

            m_defaultShader.bind();

            glm::mat4 modelMatrixChild = modelMatrixParent; // Start with the parent transformation
            modelMatrixChild = glm::translate(modelMatrixChild, glm::vec3(1.5f, 0.0f, 0.0f)); // Offset to the right of the parent
            modelMatrixChild = glm::scale(modelMatrixChild, glm::vec3(0.2f));
            modelMatrixChild = glm::rotate(modelMatrixChild, angle2, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 mvpMatrixChild = m_projectionMatrix * m_viewMatrix * modelMatrixChild;
            glm::mat3 normalModelMatrixChild = glm::inverseTranspose(glm::mat3(modelMatrixChild));

            // Set uniforms for child cube
            glUniformMatrix4fv(m_defaultShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrixChild));
            glUniformMatrix3fv(m_defaultShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrixChild));
            glUniform3fv(m_defaultShader.getUniformLocation("color"), 1, glm::value_ptr(childColor));
            glUniform1i(m_defaultShader.getUniformLocation("wolf"), false);

            // Bind texture if necessary
            if (m_meshes[1].hasTextureCoords()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_textureID);
                glUniform1i(m_defaultShader.getUniformLocation("colorMap"), 0);
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_TRUE);
                //glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), GL_FALSE);
            }
            else {
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_FALSE);
                //glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), m_useMaterial);
            }

            m_meshes[1].draw(m_defaultShader);

            // Logic for collision between wolf and cube
            AABB cubeAABB = getAABB(modelMatrixParent, cubeMinPoint, cubeMaxPoint); // Cube's AABB
            AABB wolfAABB = getAABB(m_modelMatrix_wolf, wolfMinPoint, wolfMaxPoint); // Wolf's AABB

            if (isColliding(cubeAABB, wolfAABB)) {
                if (!collisionDetected) {
                    collisionCounter++; 
                    std::cout << "Collision detected! Counter: " << collisionCounter << std::endl;
                    collisionDetected = true; 
                }
            }
            else {
                collisionDetected = false;
            }

            glDepthFunc(GL_LEQUAL); // Ensure skybox depth is always 1.0


            m_skyboxShader.bind();

            // Modify the view matrix to ignore translation (keep rotation only)
            glm::mat4 viewMatrix = glm::mat4(glm::mat3(m_viewMatrix)); 
            glUniformMatrix4fv(m_skyboxShader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(m_skyboxShader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
            glUniformMatrix4fv(m_skyboxShader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

            // Bind the cubemap texture
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Reset depth function to default
            glDepthFunc(GL_LESS);

            bool horizontal = true, first_iteration = true;
            int amount = 2;
            m_bloomShader.bind();
            glUniform1i(m_bloomShader.getUniformLocation("screenTexture"), 0);
            for (unsigned int i = 0; i < amount; i++)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                glUniform1i(m_bloomShader.getUniformLocation("horizontal"), horizontal);
                if (first_iteration)
                {
                    glBindTexture(GL_TEXTURE_2D, bloomTexture);
                    first_iteration = false;
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
                }

                glBindVertexArray(rectVAO);
                glDisable(GL_DEPTH_TEST);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                horizontal = !horizontal;
            }


            m_fboShader.bind();
            glUniform1i(m_fboShader.getUniformLocation("screenTexture"), 0);
            glUniform1i(m_fboShader.getUniformLocation("bloomTexture"), 1);
            glUniform1i(m_fboShader.getUniformLocation("inverseColor"), inverseColor);
            glUniform1i(m_fboShader.getUniformLocation("blackAndWhite"), blacnAndWhite);
            glUniform1i(m_fboShader.getUniformLocation("bloomBlur"), bloom);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindVertexArray(rectVAO);
            glDisable(GL_DEPTH_TEST); 
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
            glDrawArrays(GL_TRIANGLES, 0, 6);



            m_window.swapBuffers();
        }
    }


    // In here you can handle key presses
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyPressed(int key, int mods)
    {
        std::cout << "Key pressed: " << key << std::endl;
        float cameraSpeed = 0.1f;

        if (key == GLFW_KEY_W) {
            if (followedModel == 0) camera.moveForward();
            else if (followedModel == 1) {
                m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f)); //moves object
                camera.setFollowCharacter(m_modelMatrix_wolf);
            }
            else if (followedModel == 2) {
                m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f));
                camera.setFollowCharacter(m_modelMatrix_wolf_2);
            }
        }
        if (key == GLFW_KEY_S) {

            if (followedModel == 1) {
                m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, -cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f)); //moves object
                camera.setFollowCharacter(m_modelMatrix_wolf);
            }
            else if (followedModel == 2) {
                m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, -cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f));
                camera.setFollowCharacter(m_modelMatrix_wolf_2);
            }
            else camera.moveBack();
        }// Move backward
        if (key == GLFW_KEY_A) {
            if (followedModel == 1) {
                m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, -cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f)); //moves object
                camera.setFollowCharacter(m_modelMatrix_wolf);
            }
            else if (followedModel == 2) {
                m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, -cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f));
                camera.setFollowCharacter(m_modelMatrix_wolf_2);
            }
            else camera.moveLeft();
        }
        if (key == GLFW_KEY_D) {
            if (followedModel == 1) {
                m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f)); //moves object
                camera.setFollowCharacter(m_modelMatrix_wolf);
            }
            else if (followedModel == 2) {
                m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f));
                camera.setFollowCharacter(m_modelMatrix_wolf_2);
            }
            else camera.moveRight();
        }

        if (key == GLFW_KEY_SPACE) {
            if (followedModel == 0) camera.moveUp();
            else if (followedModel == 1) {
                m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f)); //moves object
                camera.setFollowCharacter(m_modelMatrix_wolf);
            }
            else if (followedModel == 2) {
                m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
                camera.setFollowCharacter(m_modelMatrix_wolf_2);
            }

        }

        if (key == GLFW_KEY_E) {
            if (followedModel == 0) camera.moveDown();
            else if (followedModel == 1) {
                m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, -cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f)); //moves object
                camera.setFollowCharacter(m_modelMatrix_wolf);
            }
            else if (followedModel == 2) {
                m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, -cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
                camera.setFollowCharacter(m_modelMatrix_wolf_2);
            }

        }

        if (key == GLFW_KEY_LEFT) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), -10.0f, false);
        }
        if (key == GLFW_KEY_RIGHT) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), 10.0f, false); 
        }
        if (key == GLFW_KEY_UP) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), 10.0f, true);
        }
        if (key == GLFW_KEY_DOWN) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), -10.0f, true);

        }

        if (key == GLFW_KEY_L) { 
            camera.setTopView();
        }

        m_viewMatrix = camera.viewMatrix();
    }


    // In here you can handle key releases
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyReleased(int key, int mods)
    {
        std::cout << "Key released: " << key << std::endl;
    }

    // If the mouse is moved this function will be called with the x, y screen-coordinates of the mouse
    void onMouseMove(const glm::dvec2& cursorPos)
    {
        
        if (mousePressed) {
            printf("Let's rotate!");
            glm::vec2 delta = 0.5f * glm::vec2(cursorPos - m_prevCursorPos);

            if (followedModel == 0) {
                camera.rotate(glm::vec3(m_modelMatrix[3]), delta.x, false);
                camera.rotate(glm::vec3(m_modelMatrix[3]), delta.y, true);
            }
            else if (followedModel == 1) {
                camera.rotate(glm::vec3(m_modelMatrix_wolf[3]), delta.x, false);
                camera.rotate(glm::vec3(m_modelMatrix_wolf[3]), delta.y, true);
            }
            else if (followedModel == 2) {
                camera.rotate(glm::vec3(m_modelMatrix_wolf_2[3]), delta.x, false);
                camera.rotate(glm::vec3(m_modelMatrix_wolf_2[3]), delta.y, true);
            }
            m_viewMatrix = camera.viewMatrix();
        }
        //std::cout << "Mouse at position: " << cursorPos.x << " " << cursorPos.y << std::endl;
        m_prevCursorPos = cursorPos;
    }

    // If one of the mouse buttons is pressed this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseClicked(int button, int mods)
    {

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            std::cout << "Pressed mouse button: " << button << std::endl;
            mousePressed = true;
        }
        
    }

    // If one of the mouse buttons is released this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseReleased(int button, int mods)
    {
        std::cout << "Released mouse button: " << button << std::endl;
        mousePressed = false;
        
    }

    void generateArcLengthTable(const std::vector<glm::vec3>& controlPoints) {
        int curveCount = controlPoints.size() / 4; // Each curve has 4 control points
        float totalLength = 0.0f;

        // Resize arcLengths and bezierPoints to hold points for all curves
        arcLengths.resize(curveCount * NUM_SAMPLES);
        bezierPoints.resize(curveCount * NUM_SAMPLES);

        for (int curveIndex = 0; curveIndex < curveCount; ++curveIndex) {
            // Get control points for the current curve
            const glm::vec3& p0 = controlPoints[curveIndex * 4];
            const glm::vec3& p1 = controlPoints[curveIndex * 4 + 1];
            const glm::vec3& p2 = controlPoints[curveIndex * 4 + 2];
            const glm::vec3& p3 = controlPoints[curveIndex * 4 + 3];

            // Generate points and arc lengths for this curve
            arcLengths[curveIndex * NUM_SAMPLES] = totalLength;
            bezierPoints[curveIndex * NUM_SAMPLES] = p0;
            for (int i = 1; i < NUM_SAMPLES; i++) {
                float t = static_cast<float>(i) / (NUM_SAMPLES - 1);
                glm::vec3 point = evaluateBezier(p0, p1, p2, p3, t);
                bezierPoints[curveIndex * NUM_SAMPLES + i] = point;

                // Calculate segment length and update total length
                float segmentLength = glm::length(point - bezierPoints[curveIndex * NUM_SAMPLES + i - 1]);
                totalLength += segmentLength;
                arcLengths[curveIndex * NUM_SAMPLES + i] = totalLength;
            }
        }

        // Normalize arc length values to [0, 1] across all curves
        for (float& length : arcLengths) {
            length /= totalLength;
        }
    }


    glm::vec3 getConstantSpeedPosition(float t) {
        // Find the segment where this t corresponds in arc length space
        int segment = 0;
        while (segment < arcLengths.size() - 1 && arcLengths[segment] < t) {
            segment++;
        }

        // Boundary cases: return start or end position if at extremes
        if (segment == 0) {
            return bezierPoints[0];
        }
        else if (segment >= arcLengths.size()) {
            return bezierPoints[arcLengths.size() - 1];
        }

        // Calculate local t within the segment
        float segmentT = (t - arcLengths[segment - 1]) / (arcLengths[segment] - arcLengths[segment - 1]);
        return glm::mix(bezierPoints[segment - 1], bezierPoints[segment], segmentT);
    }

    // Function to calculate the bezier curve (literally with the formula)
    glm::vec3 evaluateBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
        float u = 1.0f - t;
        return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
    }

    // Function to know if object a is colliding with object b using AABB collision boundary boxes
    bool isColliding(const AABB& a, const AABB& b) {
        return (a.x_min < b.x_max && a.x_max > b.x_min &&
            a.y_min < b.y_max && a.y_max > b.y_min);
    }

    // Function to get the boundary boxes
    AABB getAABB(const glm::mat4& modelMatrix, const glm::vec3& minPoint, const glm::vec3& maxPoint) {
        glm::vec3 min = glm::vec3(modelMatrix * glm::vec4(minPoint, 1.0f));
        glm::vec3 max = glm::vec3(modelMatrix * glm::vec4(maxPoint, 1.0f));
        return AABB{ min.x, max.x, min.y, max.y };
    }

    // Function to load the cube map
    unsigned int loadCubemap(const std::vector<std::string>& faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            Image image(faces[i]);
            if (!image.getPixels().empty()) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                    image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.getPixels().data());
            }
            else {
                std::cerr << "Failed to load cubemap texture at path: " << faces[i] << std::endl;
                throw std::runtime_error("Cubemap texture load failed");
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    // Function to load an object file, used for the wolf
    void loadObj(const std::string& filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        // Loop over shapes
        for (const auto& shape : shapes) {
            // Loop over faces (each face has 3 vertices)
            for (const auto& index : shape.mesh.indices) {
                glm::vec3 vertex(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                );
                wolfVertices.push_back(vertex);

                if (index.normal_index >= 0) {
                    glm::vec3 normal(
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    );
                    wolfNormals.push_back(normal);
                }

                if (index.texcoord_index >= 0) {
                    glm::vec2 texCoord(
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    );
                    wolfTexCoords.push_back(texCoord);
                }
            }
        }

        // Calculate the bounding box for the wolf model
        if (!wolfVertices.empty()) {
            wolfMinPoint = wolfVertices[0];
            wolfMaxPoint = wolfVertices[0];
            for (const glm::vec3& vertex : wolfVertices) {
                wolfMinPoint.x = std::min(wolfMinPoint.x, vertex.x);
                wolfMinPoint.y = std::min(wolfMinPoint.y, vertex.y);
                wolfMinPoint.z = std::min(wolfMinPoint.z, vertex.z);

                wolfMaxPoint.x = std::max(wolfMaxPoint.x, vertex.x);
                wolfMaxPoint.y = std::max(wolfMaxPoint.y, vertex.y);
                wolfMaxPoint.z = std::max(wolfMaxPoint.z, vertex.z);
            }
        }
    }


private:
    Window m_window;

    unsigned int cubemapTexture;
    unsigned int m_textureID;

    const int NUM_SAMPLES = 100;
    bool m_bezierMovementEnabled;

    // Shader for default rendering and for depth rendering
    Shader m_defaultShader;
    Shader m_shadowShader;
    Shader m_reflectionShader;
    Shader m_skyboxShader;
    Shader m_normalShader;
    Shader m_fboShader;
    Shader m_bloomShader;


    std::vector<GPUMesh> m_meshes;
    std::vector<GPUMesh> wolfMeshes;
    Texture m_texture;
    Texture m_normalMap;
    bool m_useMaterial{ false };
    bool m_normalMapping{ false };
    glm::dvec2 m_prevCursorPos;



    // Projection and view matrices for you to fill in and use
    glm::mat4 m_projectionMatrix = glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 30.0f);
    glm::mat4 m_modelMatrix{ 1.0f };
    glm::mat4 m_modelMatrix_wolf{ 1.0f };
    glm::mat4 m_modelMatrix_wolf_2{ 1.0f };
    glm::vec3 lightColor{ 1.0f };

    glm::vec3 ks{ 1.0f };
    glm::vec3 kd{ 1.0f };
    float shininess{ 16.0 };
    bool specular{ false };

    float wolf1metallic{0.4f};
    float wolf2metallic{ 0.6f};
    float wolf1roughness{0.8f};
    float wolf2roughness{0.1f};
    bool wolf1pbr{ true };
    bool wolf2pbr{ true };
    int followedModel{ 0 };

    std::vector<glm::vec3> wolfVertices;
    std::vector<glm::vec3> wolfNormals;
    std::vector<glm::vec2> wolfTexCoords;
  
    bool inverseColor{ false };
    bool blacnAndWhite{ false };
    bool bloom{ false };

    glm::vec3 cameraTarget = glm::vec3(0.0f);
    glm::vec3 cameraPos = glm::vec3(-1, 1, -1);
    Camera camera{ &m_window, cameraPos, cameraTarget };
    glm::mat4 m_viewMatrix = camera.viewMatrix();
    bool mousePressed = false;


    glm::vec3 cameraPosition;

    // Points for the bezier curve
    // Side 1: From left to top
    glm::vec3 startPoint1{ -1.0f, 0.0f, -1.0f };
    glm::vec3 controlPoint1{ -1.0f, 1.0f, -1.0f };
    glm::vec3 controlPoint2{ 0.0f, 1.0f, -1.0f };
    glm::vec3 endPoint1{ 1.0f, 0.0f, -1.0f };

    // Side 2: From top to right
    glm::vec3 startPoint2{ 1.0f, 0.0f, -1.0f };
    glm::vec3 controlPoint3{ 1.0f, 1.0f, 0.0f };
    glm::vec3 controlPoint4{ 1.0f, 1.0f, 1.0f };
    glm::vec3 endPoint2{ 1.0f, 0.0f, 1.0f };

    // Side 3: From right to bottom
    glm::vec3 startPoint3{ 1.0f, 0.0f, 1.0f };
    glm::vec3 controlPoint5{ 1.0f, -1.0f, 1.0f };
    glm::vec3 controlPoint6{ 0.0f, -1.0f, 1.0f };
    glm::vec3 endPoint3{ -1.0f, 0.0f, 1.0f };

    // Side 4: From bottom to left
    glm::vec3 startPoint4{ -1.0f, 0.0f, 1.0f };
    glm::vec3 controlPoint7{ -1.0f, -1.0f, 0.0f };
    glm::vec3 controlPoint8{ -1.0f, -1.0f, -1.0f };
    glm::vec3 endPoint4{ -1.0f, 0.0f, -1.0f };  // Same as the original startPoint to close the loop


    // For the constant move of the cube through the bezier curve
    float time = 0.0f;
    float speed = 0.1f;  // Adjust for desired speed
    float lastFrameTime = 0.0f;

    std::vector<float> arcLengths;
    std::vector<glm::vec3> bezierPoints;

    std::vector<unsigned int> m_animationTextures;

    // Animation parameters
    float m_animationSpeed;    // Time per frame
    int m_currentFrame;        // Current frame index
    float m_timeAccumulator;   // Time accumulator for frame switching

    // Bounding box for cube and wolf
    glm::vec3 cubeMinPoint = glm::vec3(-0.5f, -0.5f, -0.5f); // Adjust to cube size
    glm::vec3 cubeMaxPoint = glm::vec3(0.5f, 0.5f, 0.5f);

    glm::vec3 wolfMinPoint;
    glm::vec3 wolfMaxPoint;

    bool collisionDetected = false; // Add this as a private member in your Application class

    float m_timer{ 0.f };

    bool m_envMapEnabled = true;  // Enable environment map by default

};

int main()
{
    Application app;
    app.update();

    return 0;
}