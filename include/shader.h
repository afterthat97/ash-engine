#ifndef SHADER_H
#define SHADER_H

#include "utilities.h"

class Shader {
private:
	GLuint programID;
public:
	Shader() {
		programID = 0;
	}
	Shader(string vertexShaderPath, string fragmentShaderPath) {
		loadFromFile(vertexShaderPath, fragmentShaderPath);
	}
	~Shader() {
		glDeleteProgram(programID);
	}
	void use();
	void setBool(const string &name, bool value);
    void setInt(const string &name, int32_t value);
    void setFloat(const string &name, float value);
    void setVec2(const string &name, const vec2 &value);
    void setVec2(const string &name, float x, float y);
    void setVec3(const string &name, const vec3 &value);
    void setVec3(const string &name, float x, float y, float z);
    void setVec4(const string &name, const vec4 &value);
    void setVec4(const string &name, float x, float y, float z, float w);
    void setMat2(const string &name, const mat2 &mat);
    void setMat3(const string &name, const mat3 &mat);
    void setMat4(const string &name, const mat4 &mat);
	void loadFromFile(string vertexShaderPath, string fragmentShaderPath);
	void loadFromString(string vertexShaderCode, string fragmentShaderCode);
};

const string meshVertexShaderCode=R"(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 objPos;
out vec3 objNormalRaw;
out vec2 objTexCoord;
out mat3 TBN;

void main() {
	objPos = vec3(model * vec4(pos, 1.0));
	objNormalRaw = normal;
	objTexCoord = texCoord;

	vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
	TBN = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)";

const string meshFragmentShaderCode=R"(
#version 330 core

struct Material {
	int hasAmbientMap;
	int hasDiffuseMap;
	int hasSpecularMap;
	int hasNormalMap;
	sampler2D ambientMap;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
    vec3 ambientRGB;
    vec3 diffuseRGB;
    vec3 specularRGB;
    float shininess;
};

struct Light {
	int enable;
    vec3 position;
	vec3 color;
};

out vec4 color;

in vec3 objPos;
in vec3 objNormalRaw;
in vec2 objTexCoord;
in mat3 TBN;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform int selected;

void main() {
	vec3 ambient, diffuse, specular, objNormal = objNormalRaw;

	if (material.hasNormalMap != 0) {
		objNormal = texture(material.normalMap, objTexCoord).rgb;
		objNormal = normalize(objNormal * 2.0 - 1.0);
		objNormal = normalize(TBN * objNormal);
	}

	// ambient
	if (material.hasDiffuseMap == 0)
		ambient = vec3(0.2) * material.ambientRGB;
	else
		ambient = vec3(0.2) * vec3(texture(material.diffuseMap, objTexCoord));

    // diffuse
    vec3 lightDir = normalize(light.position - objPos);
    float diff = max(dot(objNormal, lightDir), 0.0);
	if (material.hasDiffuseMap == 0)
		diffuse = diff * light.color * material.diffuseRGB;
	else
	    diffuse = diff * light.color * vec3(texture(material.diffuseMap, objTexCoord));

    // specular
    vec3 viewDir = normalize(viewPos - objPos);
    vec3 reflectDir = reflect(-lightDir, objNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	if (material.hasSpecularMap == 0)
	    specular = spec * light.color * material.specularRGB;
	else
		specular = spec * light.color * vec3(texture(material.specularMap, objTexCoord));

    vec3 result = ambient + diffuse + specular;

	if (light.enable == 1)
		color = vec4(result, 1.0);
	else if (material.hasDiffuseMap == 1)
		color = texture(material.diffuseMap, objTexCoord);
	else
		color = vec4(material.diffuseRGB, 1.0);
	
	if (selected == 1)
		color.b += 0.5;
}
)";

const string pureColorVertexShaderCode = R"(
#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)";

const string pureColorFragmentShaderCode = R"(
#version 330 core

out vec4 color;

uniform vec4 objColor;

void main() {
    color = objColor;
}
)";

const string skyboxVertexShaderCode = R"(
#version 330 core
layout (location = 0) in vec3 pos;

out vec3 objTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
    objTexCoord = pos;
    gl_Position = projection * view * vec4(pos, 1.0);
}
)";

const string skyboxFragmentShaderCode = R"(
#version 330 core
out vec4 color;

in vec3 objTexCoord;

uniform samplerCube cubeMap;
uniform int hasCubeMap;

void main() {
    if (hasCubeMap == 1)
		color = texture(cubeMap, objTexCoord);
	else
		color = vec4(0.7, 0.7, 0.7, 1.0);
}
)";

#endif
