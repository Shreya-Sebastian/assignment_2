#version 410

out vec4 fragColor;
in vec2 texCoords;
uniform bool inverseColor;
uniform bool blackAndWhite;
uniform bool bloomBlur;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;



void main() {
	vec3 tex = texture(screenTexture, texCoords).rgb;
	vec3 bloom = texture(bloomTexture, texCoords).rgb;

	if (inverseColor) fragColor = vec4(1.0) - texture(screenTexture, texCoords);
	else if (blackAndWhite) {
		float avg = (tex.x + tex.y + tex.z) / 3.0f;
		fragColor = vec4(vec3(avg), 1.0);
	} 

	else if (bloomBlur) {
		vec3 color = tex + bloom;
		
		float exposure = 0.8f;
		vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);
		fragColor = vec4( (pow(toneMapped, vec3(1.0f / 2.2))), 1.0 );

	}

	else {
		fragColor = vec4(tex, 1.0);
	}

}