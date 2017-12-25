#include "extmath.h"

vec3 operator * (const vec3 & v1, float f) {
	return vec3(v1.x * f, v1.y * f, v1.z * f);
}

vec3 operator / (const vec3 & v1, float f) {
	return vec3(v1.x / f, v1.y / f, v1.z / f);
}

void dumpVec2(vec2 v) {
	for (uint32_t i = 0; i < 2; i++)
		printf("%12.4f", v[i]);
	putchar('\n');
}

void dumpVec3(vec3 v) {
	for (uint32_t i = 0; i < 3; i++)
		printf("%12.4f", v[i]);
	putchar('\n');
}

void dumpVec4(vec4 v) {
	for (uint32_t i = 0; i < 4; i++)
		printf("%12.4f", v[i]);
	putchar('\n');
}

void dumpMat2(mat2 m) {
	for (uint32_t i = 0; i < 2; i++) {
		for (uint32_t j = 0; j < 2; j++)
			printf("%12.4f", m[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

void dumpMat3(mat3 m) {
	for (uint32_t i = 0; i < 3; i++) {
		for (uint32_t j = 0; j < 3; j++)
			printf("%12.4f", m[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

void dumpMat4(mat4 m) {
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++)
			printf("%12.4f\t", m[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

mat3 rotate(vec3 axis, float angle) {
	axis = glm::normalize(axis);
	float cosa = cos(angle), sina = sin(angle);
	float ux = axis.x, uy = axis.y, uz = axis.z;
	mat3 ans(0.0);
	ans[0][0] = (ux * ux * (1 - cosa) + cosa);
	ans[1][0] = (ux * uy * (1 - cosa) - sina * uz);
	ans[2][0] = (ux * uz * (1 - cosa) + sina * uy);
	ans[0][1] = (ux * uy * (1 - cosa) + sina * uz);
	ans[1][1] = (uy * uy * (1 - cosa) + cosa);
	ans[2][1] = (uy * uz * (1 - cosa) - sina * ux);
	ans[0][2] = (ux * uz * (1 - cosa) - sina * uy);
	ans[1][2] = (uy * uz * (1 - cosa) + sina * ux);
	ans[2][2] = (uz * uz * (1 - cosa) + cosa);
	return ans;
}
