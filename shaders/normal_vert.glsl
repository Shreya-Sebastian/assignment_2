#version 410

uniform mat4 mvpMatrix;         // Model-View-Projection matrix
uniform mat4 modelMatrix;       // Model matrix for transforming positions
uniform mat3 normalModelMatrix; // Normal matrix for transforming normals

layout(location = 0) in vec3 position;      // Vertex position
layout(location = 1) in vec3 normal;        // Vertex normal
layout(location = 2) in vec2 texCoord;      // Texture coordinates
layout(location = 3) in vec3 tangent;       // Tangent vector
layout(location = 4) in vec3 bitangent;     // Bitangent vector

out vec3 fragPosition;   // Transformed position in world space
out vec3 fragNormal;     // Transformed normal in world space
out vec2 fragTexCoord;   // Texture coordinates
out mat3 TBN;            // TBN matrix for transforming normals from tangent space

void main()
{
    // Transform vertex position
    gl_Position = mvpMatrix * vec4(position, 1);

    // Calculate world-space position and normal
    fragPosition = (modelMatrix * vec4(position, 1)).xyz;
    fragNormal = normalize(normalModelMatrix * normal);
    fragTexCoord = texCoord;

    // Transform tangent and bitangent to world space
    vec3 T = normalize(normalModelMatrix * tangent);
    vec3 B = normalize(normalModelMatrix * bitangent);
    vec3 N = fragNormal;

    // Construct the TBN matrix
    TBN = mat3(T, B, N);
}
