#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

uniform Light light;

void main() {
    vec3 result = light.ambient + light.diffuse + light.specular;
    color = vec4(result, 1.0);
} 
