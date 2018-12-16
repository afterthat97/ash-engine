#version 330 core

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texCoords;

out vec3 fragPos;
out vec2 fragTexCoords;
out mat3 TBN;

uniform bool reverseNormal;
uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
	vec3 T = normalize(mat3(modelMat) * tangent);
	vec3 N = normalize(mat3(modelMat) * normal);
	if (reverseNormal) N = -N;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

    gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0f);
    fragPos = vec3(modelMat * vec4(position, 1.0f));
	fragTexCoords = texCoords;
	TBN = mat3(T, B, N);
}
