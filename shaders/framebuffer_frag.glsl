#version 410

out vec4 fragColor;
in vec2 texCoords;
uniform bool inverseColor;
uniform bool blackAndWhite;

uniform sampler2D screenTexture;

void main() {
	fragColor = texture(screenTexture, texCoords);

	if (inverseColor) fragColor = vec4(1.0) - texture(screenTexture, texCoords);
	else if (blackAndWhite) {
		float avg = (fragColor.x + fragColor.y + fragColor.z) / 3.0f;
		fragColor = vec4(vec3(avg), 1.0);
	
	} 

}