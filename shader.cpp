#include "shader.h"

void Shader::loadFromFile(string vertexShaderPath, string fragmentShaderPath) {
	GLuint vertexShaderID, fragmentShaderID;
	GLint compileResult = 0, compileLogLength = 0;

	// Create the shaders
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string vertexShaderCode, fragmentShaderCode;
	ifstream fin1(vertexShaderPath), fin2(fragmentShaderPath);
	if (!fin1.is_open())
		throwError("load", vertexShaderPath, "Could not open file");
	if (!fin2.is_open())
		throwError("load", fragmentShaderPath, "Could not open file");
	for (string line; getline(fin1, line);)
		vertexShaderCode += "\n" + line;
	for (string line; getline(fin2, line);)
		fragmentShaderCode += "\n" + line;
	fin1.close(); fin2.close();

	// Compile Vertex Shader
	reportInfo("Compiling shader " + string(vertexShaderPath) + "...");
	GLchar const * vertexShaderCodePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexShaderCodePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileResult);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);
	if (compileResult != 1) {
		char* compileLog = new char[compileLogLength];
		glGetShaderInfoLog(vertexShaderID, compileLogLength, NULL, compileLog);
		throwError("compile", vertexShaderPath, compileLog);
		delete[] compileLog;
	}
	reportInfo(vertexShaderPath + " has been compiled.");

	// Compile Fragment Shader
	reportInfo("Compiling shader " + string(fragmentShaderPath) + "...");
	GLchar const * fragmentShaderCodePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCodePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileResult);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);
	if (compileResult != 1) {
		char* compileLog = new char[compileLogLength];
		glGetShaderInfoLog(fragmentShaderID, compileLogLength, NULL, compileLog);
		throwError("compile", fragmentShaderPath, compileLog);
		delete[] compileLog;
	}
	reportInfo(fragmentShaderPath + " has been compiled.");

	// Link the program
	reportInfo("Linking program...");
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &compileLogLength);
	if (compileResult != 1) {
		char* compileLog = new char[compileLogLength];
		glGetProgramInfoLog(programID, compileLogLength, NULL, compileLog);
		throwError("link", "program", compileLog);
		delete[] compileLog;
	}

	// Detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	reportInfo("Shader program has been linked, ID = " + to_string(programID));
}

void Shader::loadFromString(string vertexShaderCode, string fragmentShaderCode) {
	GLuint vertexShaderID, fragmentShaderID;
	GLint compileResult = 0, compileLogLength = 0;

	// Create the shaders
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Compile Vertex Shader
	reportInfo("Compiling vertex shader...");
	GLchar const * vertexShaderCodePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexShaderCodePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileResult);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);
	if (compileResult != 1) {
		char* compileLog = new char[compileLogLength];
		glGetShaderInfoLog(vertexShaderID, compileLogLength, NULL, compileLog);
		throwError("compile", "vertex shader", compileLog);
		delete[] compileLog;
	}
	reportInfo("Vertex shader has been compiled.");

	// Compile Fragment Shader
	reportInfo("Compiling fragment shader...");
	GLchar const * fragmentShaderCodePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCodePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileResult);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);
	if (compileResult != 1) {
		char* compileLog = new char[compileLogLength];
		glGetShaderInfoLog(fragmentShaderID, compileLogLength, NULL, compileLog);
		throwError("compile", "fragment shader", compileLog);
		delete[] compileLog;
	}
	reportInfo("Fragment shader has been compiled.");

	// Link the program
	reportInfo("Linking program...");
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &compileLogLength);
	if (compileResult != 1) {
		char* compileLog = new char[compileLogLength];
		glGetProgramInfoLog(programID, compileLogLength, NULL, compileLog);
		throwError("link", "program", compileLog);
		delete[] compileLog;
	}

	// Detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	reportInfo("Shader program has been linked, ID = " + to_string(programID));
}

void Shader::use() {
	glUseProgram(programID);
}

void Shader::setBool(const string &name, bool value) {
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int32_t) value);
}

void Shader::setInt(const string &name, int32_t value) {
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) {
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVec2(const string &name, const vec2 &value) {
	glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const string &name, float x, float y) {
	glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void Shader::setVec3(const string &name, const vec3 &value) {
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const string &name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void Shader::setVec4(const string &name, const vec4 &value) {
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const string &name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const string &name, const mat2 &mat) {
	glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const string &name, const mat3 &mat) {
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const string &name, const mat4 &mat) {
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
