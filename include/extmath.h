#include "utilities.h"

vec3 operator * (const vec3& v1, float f);

vec3 operator / (const vec3& v1, float f);

void dumpVec2(vec2);

void dumpVec3(vec3);

void dumpVec4(vec4);

void dumpMat2(mat2);

void dumpMat3(mat3);

void dumpMat4(mat4);

mat3 rotate(vec3 axis, float angle);

// x1, x2, x3 are independent standard normal random variables
float stdNormalDistribution(vec3 x, vec3 o);

mat3 trianglate(mat3 s);

mat3 getRotationMat(vec3 a, vec3 b, vec3 c, vec3 ta, vec3 tb, vec3 tc);

vec3 getTranslateVec(vec3 a, vec3 b, vec3 c, vec3 ta, vec3 tb, vec3 tc);