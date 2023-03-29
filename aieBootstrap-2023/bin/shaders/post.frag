// A simple flat-color shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;
uniform int windowWidth;
uniform int windowHeight;
uniform float posterNumColors;
uniform float posterGamma;
uniform int pixels;
uniform float pixelWidth;
uniform float pixelHeight;
uniform int scanLineCount;
uniform float scanLineIntensity;
uniform float deltaTime;

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

vec4 EdgeDetection(vec2 texCoord)
{
    float w = 1.0f / windowWidth;
    float h = 1.0f / windowHeight;

    vec4 k[9];
    k[0] = texture(colorTarget, texCoord + vec2(-w, -h)); 
    k[1] = texture(colorTarget, texCoord + vec2(0, -h)); 
    k[2] = texture(colorTarget, texCoord + vec2(w, -h)); 

    k[3] = texture(colorTarget, texCoord + vec2(-w, 0)); 
    k[4] = texture(colorTarget, texCoord); 
    k[5] = texture(colorTarget, texCoord + vec2(w, 0)); 

    k[6] = texture(colorTarget, texCoord + vec2(-w, h)); 
    k[7] = texture(colorTarget, texCoord + vec2(0, h)); 
    k[8] = texture(colorTarget, texCoord + vec2(w, h)); 

    vec4 sobelEdgeH = k[2] + (2.0f * k[5]) + k[8] - (k[0] + (2.0f * k[3]) + k[6]);
    vec4 sobelEdgev = k[0] + (2.0f * k[1]) + k[2] - (k[6] + (2.0f * k[7]) + k[8]);

    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgev * sobelEdgev));

    return vec4(1.0f - sobel.rgb, 1.0f);
    
}

vec4 Sepia(vec2 texCoord)
{
    float rr = .393;
    float rg = .769;
    float rb = .189;
    
    float gr = .349;
    float gg = .686;
    float gb = .168;
    
    float br = .272;
    float bg = .534;
    float bb = .131;

    vec4 color = texture(colorTarget, texCoord);
    float r = (color.r * rr + color.g * rg + color.b * rb);
    float g = (color.r * gr + color.g * gg + color.b * gb);
    float b = (color.r * br + color.g * bg + color.b * bb);

    return vec4(r, g, b, 1.0);

}

vec4 GrayScale(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    float gray = color.r + color.g + color.b / 3;
    
    return vec4(gray, gray, gray, 1.0);
}

vec4 ScanLines(vec2 texCoord)
{
    //vec2 uv = texCoord / vec2(windowWidth, windowHeight);
    vec3 col = texture(colorTarget, texCoord).rgb;

    float scanLineYDelta = sin(deltaTime / 200.0);

    float scanLine = sin((texCoord.y - scanLineYDelta) * scanLineCount) * scanLineIntensity;

    col -= scanLine;
    return vec4(col, 1.0);
}

vec4 Invert(vec2 texCoord)
{
    return vec4(1 - texture(colorTarget, texCoord).rgb, 1.0);
}

vec4 Pixelation(vec2 texCoord)
{
    float dx = pixelWidth * (1. / pixels);
    float dy = pixelHeight * (1. / pixels);
    vec2 coord = vec2(dx * floor(texCoord.x / dx),
                   dy * floor(texCoord.y / dy));
    return texture(colorTarget, coord);
}

vec4 Posterization(vec2 texCoord)
{
    vec3 post = texture(colorTarget, texCoord).rgb;
    post = pow(post, vec3(posterGamma));
    post = post * posterNumColors;
    post = floor(post);
    post = post / posterNumColors;
    post = pow(post, vec3(1.0 / posterGamma));

    return vec4(post, 1.0);
}

vec4 Kernel(vec2 texCoord)
{
    float w = 1.0f / windowWidth;
    float h = 1.0f / windowHeight;

    float kernel[9] = float[] (
        5, -3, -3,
        5, 0, -3,
        5, -3, -3
    );

    
    vec4 sum = vec4(0);

    sum += texture(colorTarget, texCoord + vec2(-w, -h)) * kernel[0]; 
    sum += texture(colorTarget, texCoord + vec2(0, -h)) * kernel[1]; 
    sum += texture(colorTarget, texCoord + vec2(w, -h)) * kernel[2]; 

    sum += texture(colorTarget, texCoord + vec2(-w, 0)) * kernel[3]; 
    sum += texture(colorTarget, texCoord) * kernel[4]; 
    sum += texture(colorTarget, texCoord + vec2(w, 0)) * kernel[5]; 

    sum += texture(colorTarget, texCoord + vec2(-w, h)) * kernel[6]; 
    sum += texture(colorTarget, texCoord + vec2(0, h)) * kernel[7]; 
    sum += texture(colorTarget, texCoord + vec2(w, h)) * kernel[8]; 

    sum.a = 1.0;

    return sum;
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
            FragColor = EdgeDetection(texCoord);
            break;
        }
        case 3: // Grey Scale
        {
            FragColor = GrayScale(texCoord);
            break;
        }
        case 4: // Sepia
        {
            FragColor = Sepia(texCoord);
            break;
        }
        case 5: // Scanlines
        {
            FragColor = ScanLines(texCoord);
            break;
        }
        case 6: // Invert
        {
            FragColor = Invert(texCoord);
            break;
        }
        case 7: // Pixelizer
        {
            FragColor = Pixelation(texCoord);
            break;
        }
        case 8: // Posterization
        {
            FragColor = Posterization(texCoord);
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
        case 11: // Kernel
        {
            FragColor = Kernel(texCoord);
        }
    }
}