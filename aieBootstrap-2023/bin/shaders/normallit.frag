// A phong with lighting and textures shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 FragColor;

// Camera Data
uniform vec3 CameraPosition;

// Texture Data
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// Model Data
uniform vec3 Ka; // The ambient material color
uniform vec3 Kd; // The diffuse material color
uniform vec3 Ks; // The specular material color
uniform float specularPower; // The specular power of Ks

// Light data
uniform vec3 AmbientColor;
uniform vec3 LightColor;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColors[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);

    float specularTerm = pow(max(0, dot(R, view)), specularPower);
    return specularTerm * color;
}

void main()
{
    // Set the normal and light direction
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    mat3 TBN = mat3(T, B, N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = TBN * (texNormal * 2 - 1);

    // Calculate the negative light direction (Lambert Term)
    float lambertTerm = max(0 ,min(1, dot(N, -L)));

    // Calculate the view vector...
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    // ..and reflection vector
    vec3 R = reflect(L, N);

    // Calculate the diffuse value of light from the global source
    vec3 diffuseTotal = Diffuse(L, LightColor, N);

    // Calculate the specular value of light from the global source
    vec3 specularTotal = Specular(L, LightColor, N, V);

    for (int i = 0; i < numLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float distance = length(direction);
        direction = direction / distance;

        // Set the lighting intensity with the inverse square law
        vec3 color = PointLightColors[i] / (distance * distance);

        diffuseTotal += Diffuse(direction, color, N);
        specularTotal += Specular(direction, color, N, V);
    }

    // Calculate the propeorties of each color type
    vec3 ambient = AmbientColor * Ka * texNormal;
    vec3 diffuse = diffuseTotal * Kd * texDiffuse;
    vec3 specular = specularTotal * Ks * texSpecular;

    FragColor = vec4(ambient + diffuse + specular, 1);
}