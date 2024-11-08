#version 410
out vec4 fragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform bool horizontal;

const int radius = 6;
float spreadBlur = 2.0f;
float weights[radius];

void main()
{             
    // based on two pass gausian blur
    float x = 0.0f;
    for (int i = 0; i < radius; i++)
    {
        if (spreadBlur <= 2.0f)
            x += 3.0f / radius;
        else
            x += 6.0f / radius;

        weights[i] = exp(-0.5f * pow(x / spreadBlur, 2.0f)) / (spreadBlur * sqrt(2 * 3.14159265f));
    }


    vec2 tex_offset = 1.0f / textureSize(screenTexture, 0);
    vec3 result = texture(screenTexture, texCoords).rgb * weights[0];

    if(horizontal)
    {
        for(int i = 1; i < radius; i++)
        {
            result += texture(screenTexture, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
            result += texture(screenTexture, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
        }
    }
    else
    {
        for(int i = 1; i < radius; i++)
        {
            result += texture(screenTexture, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weights[i];
            result += texture(screenTexture, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weights[i];
        }
    }
    fragColor = vec4(result, 1.0f);
}