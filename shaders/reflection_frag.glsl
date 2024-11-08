#version 410 core
out vec4 fragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

void main() {
    vec3 I = normalize(Position - cameraPosition); // View direction
    vec3 R = reflect(I, normalize(Normal));   // Reflection vector

    fragColor = vec4(texture(skybox, R));
}
