#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
uniform sampler2D texture0;
uniform Material material;
uniform Light light;

void main() {
	vec3 objectColor = texture(texture0, objectTexCoord).rgb;

	// ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(objectNormal);
    vec3 lightDir = normalize(light.position - objectPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;
    
    // specular
    vec3 viewDir = normalize(viewPos - objectPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}
