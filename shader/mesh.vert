#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 PVM;

out vec3 objectPos;
out vec3 objectNormalRaw;
out vec2 objectTexCoord;
out mat3 TBN;

void main() {
	objectPos = aPos;
	objectNormalRaw = aNormal;
	objectTexCoord = aTexCoord;

	vec3 T = normalize(aTangent);
	vec3 N = normalize(aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(T, N));
	TBN = mat3(T, B, N);

	gl_Position = PVM * vec4(aPos, 1.0f);
}
