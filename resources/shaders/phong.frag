#version 330 core

out vec4 fragColor;

in vec3 fragPos;
in vec2 fragTexCoords;
in mat3 TBN;

struct PhongMaterial {
    vec3 color;
    vec3 bump;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D bumpMap;
};

struct AmbientLight {
    vec3 color;
};

struct DirectionalLight {
    vec3 color;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    vec3 pos;
    float enableAttenuation;
    float attenuationQuadratic;
    float attenuationLinear;
    float attenuationConstant;
};

struct SpotLight {
    vec3 color;
    vec3 pos;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
    float enableAttenuation;
    float attenuationQuadratic;
    float attenuationLinear;
    float attenuationConstant;
};

uniform int ambientLightNum;
uniform AmbientLight ambientLight[8];

uniform int directionalLightNum;
uniform DirectionalLight directionalLight[8];

uniform int pointLightNum;
uniform PointLight pointLight[8];

uniform int spotLightNum;
uniform SpotLight spotLight[8];

uniform vec3 viewPos;
uniform PhongMaterial material;

vec3 calcAmbientLight(int idx, vec3 color, float ambient) {
    return ambient * color * ambientLight[idx].color;
}

vec3 calcDirectionalLight(int idx, vec3 normal, vec3 color, float diff, float spec) {
    vec3 lightDir = normalize(-directionalLight[idx].direction);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 result = vec3(0.0f);
    result += diff * color * max(dot(normal, lightDir), 0.0f);
    result += spec * color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    return result * directionalLight[idx].color;
}

vec3 calcPointLight(int idx, vec3 normal, vec3 color, float diff, float spec) {
    vec3 lightDir = normalize(pointLight[idx].pos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float dis = length(pointLight[idx].pos - fragPos);

    vec3 result = vec3(0.0f);
    result += diff * color * max(dot(normal, lightDir), 0.0f);
    result += spec * color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float attenuation = 1 / (pointLight[idx].attenuationConstant
                            + pointLight[idx].attenuationLinear * dis
                            + pointLight[idx].attenuationQuadratic * dis * dis);
    result *= attenuation * pointLight[idx].enableAttenuation
            + (1 - pointLight[idx].enableAttenuation);

    return result * pointLight[idx].color;
}

vec3 calcSpotLight(int idx, vec3 normal, vec3 color, float diff, float spec) {
    vec3 lightDir = normalize(spotLight[idx].pos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float dis = length(spotLight[idx].pos - fragPos);
    float theta = dot(lightDir, normalize(-spotLight[idx].direction));
    float intensity = (theta - spotLight[idx].outerCutOff) / (spotLight[idx].innerCutOff - spotLight[idx].outerCutOff);

    vec3 result = vec3(0.0f);
    result += diff * color * max(dot(normal, lightDir), 0.0f);
    result += spec * color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float attenuation = 1 / (spotLight[idx].attenuationConstant
                            + spotLight[idx].attenuationLinear * dis
                            + spotLight[idx].attenuationQuadratic * dis * dis);
    result *= attenuation * spotLight[idx].enableAttenuation
            + (1 - spotLight[idx].enableAttenuation);

    return result * spotLight[idx].color * clamp(intensity, 0.0, 1.0);
}

void main() {
    vec3 color  = texture(material.diffuseMap, fragTexCoords).rgb + material.color;
    float spec  = texture(material.specularMap, fragTexCoords).r + material.specular;
    vec3 normal = texture(material.bumpMap, fragTexCoords).rgb + material.bump;
    normal = normalize(TBN * (normal * 2 - 1));

    fragColor = vec4(0, 0, 0, 1);

    for (int i = 0; i < ambientLightNum; i++)
        fragColor += vec4(calcAmbientLight(i, color, material.ambient), 1);

    for (int i = 0; i < directionalLightNum; i++)
        fragColor += vec4(calcDirectionalLight(i, normal, color, material.diffuse, spec), 1);

    for (int i = 0; i < pointLightNum; i++)
        fragColor += vec4(calcPointLight(i, normal, color, material.diffuse, spec), 1);

    for (int i = 0; i < spotLightNum; i++)
        fragColor += vec4(calcSpotLight(i, normal, color, material.diffuse, spec), 1);
}
