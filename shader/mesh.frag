#version 330 core

struct Material {
	int hasAmbientMap;
	int hasDiffuseMap;
	int hasSpecularMap;
	sampler2D ambientMap;
	sampler2D diffuseMap;
	sampler2D specularMap;
    vec3 ambientRGB;
    vec3 diffuseRGB;
    vec3 specularRGB;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 objectNormal;
in vec3 objectPos;
in vec2 objectTexCoord;
  
uniform vec3 viewPos; 
uniform Material material;
uniform Light light;

void main() {
	vec3 ambient, diffuse, specular;

	// ambient
	if (material.hasAmbientMap == 0 && material.hasDiffuseMap == 0)
		ambient = light.ambient * material.ambientRGB; 
	else if (material.hasAmbientMap == 0)
		ambient = light.ambient * vec3(texture(material.diffuseMap, objectTexCoord));
	else
	    ambient = light.ambient * vec3(texture(material.ambientMap, objectTexCoord));
  	
    // diffuse 
    vec3 norm = normalize(objectNormal);
    vec3 lightDir = normalize(light.position - objectPos);
    float diff = max(dot(norm, lightDir), 0.0);
	if (material.hasDiffuseMap == 0)
		diffuse = diff * light.diffuse * material.diffuseRGB;
	else
	    diffuse = diff * light.diffuse * vec3(texture(material.diffuseMap, objectTexCoord));
    
    // specular
    vec3 viewDir = normalize(viewPos - objectPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	if (material.hasSpecularMap == 0)
	    specular = spec * light.specular * material.specularRGB;
	else
		specular = spec * light.specular * vec3(texture(material.specularMap, objectTexCoord));

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
	color = texture(material.diffuseMap, objectTexCoord);
}
