#version 330 core

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoords;

out vec3 fragPos;
out vec2 fragTexCoords;
out mat3 TBN;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
	vec3 T = normalize(vec3(modelMat * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(modelMat * vec4(bitangent,   0.0)));
	vec3 N = normalize(vec3(modelMat * vec4(normal,    0.0)));

    gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0f);
    fragPos = vec3(modelMat * vec4(position, 1.0f));
	fragTexCoords = texCoords;
	TBN = mat3(T, B, N);
}
