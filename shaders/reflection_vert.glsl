#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    Position = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal; // Correct normal for non-uniform scaling
    gl_Position = projection * view * model * vec4(position, 1.0);
}
