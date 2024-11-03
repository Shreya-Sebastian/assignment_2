#version 410

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
// Normals should be transformed differently than positions:
// https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
uniform mat3 normalModelMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragTangent;
out vec3 fragBitangent;

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1);
    
    fragPosition    = (modelMatrix * vec4(position, 1)).xyz;
    fragNormal      = normalModelMatrix * normal;
    fragTexCoord    = texCoord;

    // Calculate tangent and bitangent vectors
    // Assuming access to adjacent vertices and texture coordinates

    vec3 deltaPos1 = vec3(1.0, 0.0, 0.0); // placeholder for adjacent vertex position difference
    vec3 deltaPos2 = vec3(0.0, 1.0, 0.0); // placeholder for adjacent vertex position difference

    vec2 deltaUV1 = vec2(1.0, 0.0); // placeholder for adjacent texture coordinate difference
    vec2 deltaUV2 = vec2(0.0, 1.0); // placeholder for adjacent texture coordinate difference

    float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

    fragTangent = normalize(normalModelMatrix * tangent);
    fragBitangent = normalize(normalModelMatrix * bitangent);
}
