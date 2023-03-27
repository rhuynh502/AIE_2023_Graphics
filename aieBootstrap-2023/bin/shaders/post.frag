// A simple flat-color shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;

out vec4 FragColor;

vec4 Default(vec2 texCoord)
{
    return texture(colorTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0);

    vec4 color = texture(colorTarget, texCoord);
    color += texture(colorTarget, texCoord + texel * vec2(-1, 1));
    color += texture(colorTarget, texCoord + texel * vec2(-1, 0));
    color += texture(colorTarget, texCoord + texel * vec2(-1, -1));

    color += texture(colorTarget, texCoord + texel * vec2(0, 1));
    color += texture(colorTarget, texCoord + texel * vec2(0, -1));

    color += texture(colorTarget, texCoord + texel * vec2(1, 1));
    color += texture(colorTarget, texCoord + texel * vec2(1, -1));
    color += texture(colorTarget, texCoord + texel * vec2(1, 0));

    return color / 9;
}

vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromTheCenter = distance(texCoord, mid);
    vec2 normalizeCoord = normalize(texCoord - mid);
    float bias = distanceFromTheCenter + sin(distanceFromTheCenter * 15) * 0.05;
    vec2 newCoord = mid + bias * normalizeCoord;
    return texture(colorTarget, newCoord);
}

vec4 Flip(vec2 texCoord)
{
    return vec4(1.0f + texture(colorTarget, texCoord).rgb, 1.0f);
}

void main()
{
    // This calculates the texel size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Then we will adjust the coordinates 
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    switch(postProcessTarget)
    {
        default: // Default
        {
            // Output post process effect here
            FragColor = Default(texCoord);
            break;
        }
        case 0: // BoxBlur
        {
            FragColor = BoxBlur(texCoord);
            break;
        }
        case 1: // Distort
        {
            FragColor = Distort(texCoord);
            break;
        }
        case 2: // Edge Detection
        {
            FragColor = Default(texCoord);
            break;
        }
        case 3: // Grey Scale
        {
            FragColor = Default(texCoord);
            break;
        }
        case 4: // Sepia
        {
            FragColor = Default(texCoord);
            break;
        }
        case 5: // Scanlines
        {
            FragColor = Default(texCoord);
            break;
        }
        case 6: // Invert
        {
            FragColor = Default(texCoord);
            break;
        }
        case 7: // Pixelizer
        {
            FragColor = Default(texCoord);
            break;
        }
        case 8: // Posterization
        {
            FragColor = Default(texCoord);
            break;
        }
        case 9: // Distance Fog
        {
            FragColor = Default(texCoord);
            break;
        }
        case 10: // Depth Of Field
        {
            FragColor = Default(texCoord);
            break;
        }
        case 11: // Flip
        {
            FragColor = Flip(texCoord);
        }
    }
}