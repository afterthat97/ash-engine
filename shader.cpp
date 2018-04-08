#include "shader.h"

const string meshVertexShaderCode = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
	vec3 fragPos;
	vec2 texCoords;
	mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vs_out.fragPos = vec3(model * vec4(position, 1.0f));
	vs_out.texCoords = texCoords;
	
	vec3 T = normalize(mat3(model) * tangent);
	vec3 N = normalize(mat3(model) * normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);
}
)";

const string meshFragmentShaderCode = R"(
#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec3 fragPos;
	vec2 texCoords;
	mat3 TBN;
} fs_in;

struct Material {
	bool hasDiffuseMap;
	bool hasSpecularMap;
	bool hasNormalMap;
	bool hasParallaxMap;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D parallaxMap;
	vec3 ambientRGB;
	vec3 diffuseRGB;
	vec3 specularRGB;
	float shininess;
};

struct Light {
	vec3 color;
	vec3 pos;
	bool hasDepthMap;
	float farPlane;
};

uniform int lightNum;
uniform int selected;
uniform int enableLight;
uniform int enableAttenuation;
uniform vec3 viewPos;

uniform Material material;
uniform Light lights[8];
uniform samplerCube depthMap[8];

vec4 calcPointLight(int idx, vec3 diff, vec3 spec, vec3 normal) {
	float distance = length(lights[idx].pos - fs_in.fragPos);
	float attenuation = 1.0f / (1.0f + 0.0014f * distance + 0.000007f * (distance * distance));
	vec3 lightDir = normalize(lights[idx].pos - fs_in.fragPos);
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	vec3 res = diff * max(dot(normal, lightDir), 0.0f);
	res += spec * pow(max(dot(normal, halfwayDir), 0.0f), material.shininess * 4.0f);
	if (enableAttenuation == 1) res *= attenuation;
	return vec4(res * lights[idx].color, 1.0f);
}

vec2 ParallaxMapping(vec2 texCoords) {
	vec3 viewDir = transpose(fs_in.TBN) * normalize(viewPos - fs_in.fragPos);
	float minLayers = 8.0f, maxLayers = 64.0f, height_scale = 0.02f;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));
	float layerDepth = 1.0f / numLayers;
	float currentLayerDepth = 0.0f;
	vec2 P = viewDir.xy / viewDir.z * height_scale; 
	vec2 deltaTexCoords = P / numLayers, currentTexCoords = texCoords;
	float currentDepthMapValue = 1.0f - texture(material.parallaxMap, currentTexCoords).r;
	while (currentLayerDepth < currentDepthMapValue) {
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = 1.0f - texture(material.parallaxMap, currentTexCoords).r;  
		currentLayerDepth += layerDepth;  
	}
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1.0f - texture(material.parallaxMap, prevTexCoords).r - currentLayerDepth + layerDepth;
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 dispTexCoords = prevTexCoords * weight + currentTexCoords * (1.0f - weight);
	if (dispTexCoords.y > 1.0f || dispTexCoords.y < 0.0f || dispTexCoords.x > 1.0f || dispTexCoords.x < 0.0f) discard;
	return dispTexCoords;
}

vec3 sampleOffsetDirections[20] = vec3[] (
	vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
	vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
	vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
	vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float calcPointLightShadow(int idx, vec3 normal) {
	if (lights[idx].hasDepthMap == false) return 0.0f;
	vec3 fragToLight = fs_in.fragPos - lights[idx].pos;
	vec3 lightDir = normalize(lights[idx].pos - fs_in.fragPos);
	float currentDepth = length(fragToLight);
	float shadow = 0.0f;
	float bias = max(5.0f * (1.0f - dot(normal, lightDir)), 0.5f);
	float radius = 0.5f;
	for (int i = 0; i < 20; ++i) {
		vec3 dir = fragToLight + sampleOffsetDirections[i] * radius;
		float closestDepth = lights[idx].farPlane;
		switch (idx) {
			case 0: closestDepth *= texture(depthMap[0], dir).r; break;
			case 1: closestDepth *= texture(depthMap[1], dir).r; break;
			case 2: closestDepth *= texture(depthMap[2], dir).r; break;
			case 3: closestDepth *= texture(depthMap[3], dir).r; break;
			case 4: closestDepth *= texture(depthMap[4], dir).r; break;
			case 5: closestDepth *= texture(depthMap[5], dir).r; break;
			case 6: closestDepth *= texture(depthMap[6], dir).r; break;
			case 7: closestDepth *= texture(depthMap[7], dir).r; break;
		}
		if (currentDepth - bias > closestDepth) shadow += 1.0f;
	}
	return shadow / 20.0f;
}

void main() {
	fragColor = vec4(1.0f);
	vec2 dispTexCoords = (material.hasParallaxMap ? ParallaxMapping(fs_in.texCoords) : fs_in.texCoords);
	vec3 diffuseColor  = (material.hasDiffuseMap  ? texture(material.diffuseMap, dispTexCoords).rgb : material.diffuseRGB);
	vec3 specularColor = (material.hasSpecularMap ? texture(material.specularMap, dispTexCoords).rgb : material.specularRGB);
	vec3 normal		   = (material.hasNormalMap   ? texture(material.normalMap, dispTexCoords).rgb * 2.0f - 1.0f : vec3(0.0f, 0.0f, 1.0f));
	normal = fs_in.TBN * normalize(normal);

	if (enableLight == 0) {
		fragColor = vec4(diffuseColor, 1.0f);
	} else {
		fragColor = vec4(diffuseColor * material.ambientRGB, 1.0f);
		for (int i = 0; i < lightNum; i++) {
			float shadowStrength = (1.0f - calcPointLightShadow(i, normal));
			fragColor += shadowStrength * calcPointLight(i, diffuseColor, specularColor, normal);
		}
	}
	if (selected == 1) fragColor.b += 0.5f;
}
)";

const string depthVertexShaderCode = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main() {
	gl_Position = model * vec4(aPos, 1.0f);
}
)";

const string depthFragmentShaderCode = R"(
#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
	float lightDistance = length(FragPos.xyz - lightPos);
	lightDistance = lightDistance / farPlane;
	gl_FragDepth = lightDistance;
}
)";

const string depthGeometryShaderCode = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main() {
	for (int face = 0; face < 6; ++face) {
		gl_Layer = face;
		for (int i = 0; i < 3; ++i) {
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
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
	gl_Position = projection * view * vec4(pos, 1.0f);
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
		color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
}
)";

string loadStringFromFile(string path) {
    if (path == "") return "";
    ifstream fin(path);
    if (!fin.is_open()) {
        reportError("Failed to open " + path);
        return "";
    }
    string str;
    for (string line; getline(fin, line);)
        str += "\n" + line;
    fin.close();
    return str;
}

bool Shader::loadFromFile(string vertexShaderPath, string fragmentShaderPath, string geometryShaderPath) {
    // Read the Vertex Shader code from the file
    string vertexShaderCode = loadStringFromFile(vertexShaderPath);
    string fragmentShaderCode = loadStringFromFile(fragmentShaderPath);
    string geometryShaderCode = loadStringFromFile(geometryShaderPath);
    return loadFromString(vertexShaderCode, fragmentShaderCode, geometryShaderCode);
}

bool Shader::loadFromString(string vertexShaderCode, string fragmentShaderCode, string geometryShaderCode) {
    GLuint vertexShaderID, fragmentShaderID, geometryShaderID;
    GLint compileResult = 0, compileLogLength = 0;

    // Create the shaders
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (geometryShaderCode != "")
        geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

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
        reportError("Failed to compile vertex shader: " + string(compileLog));
        delete[] compileLog;
        return false;
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
        reportError("Failed to compile fragment shader: " + string(compileLog));
        delete[] compileLog;
        return false;
    }
    reportInfo("Fragment shader has been compiled.");

    if (geometryShaderCode != "") {
        // Compile Geometry Shader
        reportInfo("Compiling geometry shader...");
        GLchar const * geometryShaderCodePointer = geometryShaderCode.c_str();
        glShaderSource(geometryShaderID, 1, &geometryShaderCodePointer, NULL);
        glCompileShader(geometryShaderID);

        // Check geometry Shader
        glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &compileResult);
        glGetShaderiv(geometryShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);
        if (compileResult != 1) {
            char* compileLog = new char[compileLogLength];
            glGetShaderInfoLog(geometryShaderID, compileLogLength, NULL, compileLog);
            reportError("Failed to compile geometry shader: " + string(compileLog));
            delete[] compileLog;
            return false;
        }
        reportInfo("Geometry shader has been compiled.");
    }

    // Link the program
    reportInfo("Linking program...");
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    if (geometryShaderCode != "")
        glAttachShader(programID, geometryShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &compileLogLength);
    if (compileResult != 1) {
        char* compileLog = new char[compileLogLength];
        glGetProgramInfoLog(programID, compileLogLength, NULL, compileLog);
        reportError("Failed to link program: " + string(compileLog));
        delete[] compileLog;
        return false;
    }

    // Detach shader
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    if (geometryShaderCode != "") glDetachShader(programID, geometryShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if (geometryShaderCode != "") glDeleteShader(geometryShaderID);
    reportInfo("Shader program has been linked, ID = " + to_string(programID));
	return true;
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
