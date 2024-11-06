#version 410

layout(std140) uniform Material // Must match the GPUMaterial defined in src/mesh.h
{
    vec3 kd;
	vec3 ks;
	float shininess;
	float transparency;
};

uniform sampler2D colorMap;
uniform bool hasTexCoords;
uniform bool useMaterial;
uniform vec3 color;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 objectColor = color;

    if (useMaterial) { 
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (hasTexCoords) { 
        fragColor = texture(colorMap, fragTexCoord) * vec4(objectColor, 1.0);   
    }
    else { 
        fragColor = vec4(objectColor, 1.0);
    }
}
