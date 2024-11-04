#version 410 core
out vec4 fragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;
//uniform vec3 baseColor; // Add a base color for the object
//uniform float reflectionStrength; // Controls reflection intensity (0 to 1)

void main() {
    vec3 I = normalize(Position - cameraPosition); // View direction
    vec3 R = reflect(I, normalize(Normal));   // Reflection vector
    //vec3 reflection = texture(skybox, R).rgb;
    //vec3 color = mix(baseColor, reflection, reflectionStrength);
    fragColor = vec4(texture(skybox, R));
}
