out vec4 fragColor;

in vec3 fragPos;
in vec2 fragTexCoords;
in mat3 TBN;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D bumpMap;

vec3 calcAmbientLight(int idx, vec3 color, float ambient) {
    return ambient * color * vec3(ambientLight[idx].color);
}

vec3 calcDirectionalLight(int idx, vec3 normal, vec3 color, float diff, float spec) {
    vec3 lightDir = normalize(-vec3(directionalLight[idx].direction));
    vec3 viewDir = normalize(vec3(viewPos) - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 result = vec3(0.0f);
    result += diff * color * max(dot(normal, lightDir), 0.0f);
    result += spec * color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    return result * vec3(directionalLight[idx].color);
}

vec3 calcPointLight(int idx, vec3 normal, vec3 color, float diff, float spec) {
    vec3 lightDir = normalize(vec3(pointLight[idx].pos) - fragPos);
    vec3 viewDir = normalize(vec3(viewPos) - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float dis = length(vec3(pointLight[idx].pos) - fragPos);

    vec3 result = vec3(0.0f);
    result += diff * color * max(dot(normal, lightDir), 0.0f);
    result += spec * color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float attenuation = 1 / (pointLight[idx].attenuationConstant
                            + pointLight[idx].attenuationLinear * dis
                            + pointLight[idx].attenuationQuadratic * dis * dis);
    result *= attenuation * pointLight[idx].enableAttenuation
            + (1 - pointLight[idx].enableAttenuation);

    return result * vec3(pointLight[idx].color);
}

vec3 calcSpotLight(int idx, vec3 normal, vec3 color, float diff, float spec) {
    vec3 lightDir = normalize(vec3(spotLight[idx].pos) - fragPos);
    vec3 viewDir = normalize(vec3(viewPos) - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float dis = length(vec3(spotLight[idx].pos) - fragPos);
    float theta = dot(lightDir, normalize(-vec3(spotLight[idx].direction)));
    float intensity = (theta - spotLight[idx].outerCutOff) / (spotLight[idx].innerCutOff - spotLight[idx].outerCutOff);

    vec3 result = vec3(0.0f);
    result += diff * color * max(dot(normal, lightDir), 0.0f);
    result += spec * color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float attenuation = 1 / (spotLight[idx].attenuationConstant
                            + spotLight[idx].attenuationLinear * dis
                            + spotLight[idx].attenuationQuadratic * dis * dis);
    result *= attenuation * spotLight[idx].enableAttenuation
            + (1 - spotLight[idx].enableAttenuation);

    return result * vec3(spotLight[idx].color) * clamp(intensity, 0.0, 1.0);
}

void main() {
    vec3 color  = material.useDiffuseMap == 1 ? texture(diffuseMap, fragTexCoords).rgb : vec3(material.color);
    float spec  = material.useSpecularMap == 1 ? texture(specularMap, fragTexCoords).r : material.specular;
    vec3 normal = material.useBumpMap == 1 ? texture(bumpMap, fragTexCoords).rgb * 2 - 1 : vec3(0, 0, 1);
    normal = normalize(TBN * normalize(normal));

    fragColor = vec4(0, 0, 0, 1);

    for (int i = 0; i < ambientLightNum; i++)
        fragColor += vec4(calcAmbientLight(i, color, material.ambient), 1);

    for (int i = 0; i < directionalLightNum; i++)
        fragColor += vec4(calcDirectionalLight(i, normal, color, material.diffuse, spec), 1);

    for (int i = 0; i < pointLightNum; i++)
        fragColor += vec4(calcPointLight(i, normal, color, material.diffuse, spec), 1);

    for (int i = 0; i < spotLightNum; i++)
        fragColor += vec4(calcSpotLight(i, normal, color, material.diffuse, spec), 1);
    
    if (highlighted == 1)
        fragColor += vec4(0.2, 0.2, 0.2, 0);
    
    if (selected == 1)
        fragColor += vec4(0, 0, 0.4, 0);
}
