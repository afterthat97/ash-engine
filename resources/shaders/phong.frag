#version 330 core

out vec4 fragColor;

in vec3 fragPos;
in vec2 fragTexCoords;
in mat3 TBN;

struct Material {
    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasNormalMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    vec3 ambientRGB;
    vec3 diffuseRGB;
    vec3 specularRGB;
    float shininess;
};

struct Light {
    vec3 color;
    vec3 pos;
    bool enableAttenuation;
    float attenuationQuadratic;
    float attenuationLinear;
    float attenuationConstant;
};

uniform vec3 viewPos;
uniform bool enableLighting;
uniform int lightNum;
uniform Light lights[8];
uniform Material material;

vec4 calcPointLight(int idx, vec3 diffuseColor, vec3 specularColor, vec3 normal) {
	vec3 lightDir = normalize(lights[idx].pos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
    
	vec3 result = vec3(0.0f);
    result += diffuseColor * max(dot(normal, lightDir), 0.0f);
    result += specularColor * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    if (lights[idx].enableAttenuation) {
		float dis = length(lights[idx].pos - fragPos);
		result /= (lights[idx].attenuationConstant + lights[idx].attenuationLinear * dis + lights[idx].attenuationQuadratic * dis * dis);
	}
	return vec4(result * lights[idx].color, 1.0f);
}

void main() {
    vec3 diffuseColor = (material.hasDiffuseMap ? texture(material.diffuseMap, fragTexCoords).rgb : material.diffuseRGB);
    vec3 specularColor = (material.hasSpecularMap ? texture(material.specularMap, fragTexCoords).rgb : material.specularRGB);
    vec3 normal = (material.hasNormalMap ? texture(material.normalMap, fragTexCoords).rgb * 2.0f - 1.0f : vec3(0.0f, 0.0f, 1.0f));
    normal = TBN * normalize(normal);

    if (enableLighting) {
        fragColor = vec4(diffuseColor * material.ambientRGB, 1.0f);
        for (int i = 0; i < lightNum; i++)
            fragColor += calcPointLight(i, diffuseColor, specularColor, normal);
    } else {
        fragColor = vec4(diffuseColor, 1.0f);
    }
}
