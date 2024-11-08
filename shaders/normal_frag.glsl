#version 410



uniform sampler2D colorMap;
uniform sampler2D normalMap;


uniform bool hasTexCoords;
uniform bool normalMapping;
uniform vec3 color;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in mat3 TBN;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform bool wolf;
uniform float metallic;
uniform float roughness;
uniform vec3 lightColor;

uniform vec3 ks;
uniform vec3 kd;
uniform float shininess;
uniform bool specular;

const float PI = 3.14159265359;


layout(location = 0) out vec4 fragColor;
layout (location = 1) out vec4 bloomColor;


float DistributionGGX(vec3 N,vec3 H, float roughness) {
    float a  = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    
    float denominator = (NdotH*NdotH * (a2 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;
	
    return a2 / denominator;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) /8.0;
    float denominator = NdotV * (1.0 - k) + k;
	
    return NdotV / denominator;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2= GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0){
  return F0 + (1.0 - F0)* pow(clamp(1.0- cosTheta, 0.0,  1.0) ,5.0);
} 


void main()
{

    vec3 normal = normalize(fragNormal);
    vec3 V = normalize(cameraPos - fragPosition);
    vec3 L = lightPos;
    vec3 objectColor = color;

    vec3 light_dir = normalize(L - fragPosition);

    vec3 Nnormal = texture(normalMap, fragTexCoord).rgb;
    Nnormal = Nnormal * 2.0 - 1.0;
    Nnormal = normalize(Nnormal);

    float NdotL = max(dot(normal, light_dir), 0.0);    
    float NNdotL = NdotL;

    if (normalMapping){
        //TBN = transpose(mat3(T, B, N));
        NNdotL = max(dot(Nnormal, light_dir), 0.0);  ;  
        //normal = normalize(Nnormal) ;  

    }

    if (hasTexCoords) 
    { 
        fragColor = texture(colorMap, fragTexCoord) * vec4(objectColor, 1.0);   
    }
    else if (wolf) 
    { 
        
        vec3 Lo = vec3(0.0);

        vec3 lightDir = normalize(lightPos-fragPosition);
        vec3 H = normalize(V + lightDir);
        float distance = length(lightPos - fragPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColor * attenuation;
        vec3 F0 = vec3(0.04); 
        F0 = mix(F0, objectColor, metallic);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        float NDF= DistributionGGX(normal, H, roughness);       
        float G = GeometrySmith(normal, V, lightDir, roughness);  
        
        vec3 numerator= NDF * G * F;
        float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, lightDir), 0.0)  + 0.0001;
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
  
        kD *= 1.0 - metallic;	

        
   

        Lo += (kD * objectColor / PI + specular) * radiance * NdotL;

        vec3 ambient = vec3(0.005) * objectColor;
        Lo   = ambient + Lo;
        Lo = Lo / (Lo + vec3(1.0));
        Lo = pow(Lo, vec3(1.0/2.2)); 


        fragColor = NNdotL * vec4(Lo, 1.0);
        //fragColor = vec4(normal.xz,0.f, 1.0);
    } 
    else 
    {
       // simple lighting 
        vec3 lightDir = normalize(lightPos - fragPosition);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = kd * diff * lightColor * objectColor;
        vec3 ambientColor = vec3(0.1, 0.1, 0.1); 
        vec3 ambient = ambientColor * lightColor * objectColor; 
        vec3 accumulatedColor = diffuse + ambient;

        if (specular) {
           vec3 R = reflect(-lightDir, normal);
            vec3 V = normalize(cameraPos - fragPosition);
            float spec = max(dot(R, V), 0.0); 
            vec3 specular = ks * objectColor * pow(spec, shininess);
            accumulatedColor += specular * lightColor;
        }

        fragColor = vec4(accumulatedColor, 1.0);
        }

    // pass to bloom texture
    vec3 someColor = fragColor.rgb;
    if (someColor.r > 0.05f)
    someColor.r *= 5.0f;

	// Calculate brightness by adding up all the channels with different weights each
	float brightness = dot(someColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 0.15f)
        bloomColor = vec4(someColor.rgb, 1.0f);
    else
        bloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        

}