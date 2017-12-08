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
};

#endif
