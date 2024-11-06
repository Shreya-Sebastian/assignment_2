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
uniform sampler2D normalMap;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 light_pos = {0.f, 0.f, 20.f};
    vec3 light_dir = normalize(fragPosition - light_pos);

    vec3 normal = texture(normalMap, fragTexCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(-1* normal);

    float NdotL = dot(normal, light_dir);


    if (hasTexCoords)       { fragColor = NdotL * vec4(texture(colorMap, fragTexCoord).rgb, 1);}
    else if (useMaterial)   { fragColor = NdotL * vec4(kd, 1);}
    else                    { fragColor = NdotL * vec4(normal, 1); } // Output color value, change from (1, 0, 0) to something else

}
