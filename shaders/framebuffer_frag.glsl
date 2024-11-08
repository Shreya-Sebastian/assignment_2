#version 410

out vec4 fragColor;
in vec2 texCoords;
uniform bool inverseColor;

uniform sampler2D screenTexture;

void main() {
	fragColor = texture(screenTexture, texCoords);
	if (inverseColor) fragColor = vec4(1.0) - texture(screenTexture, texCoords);
}