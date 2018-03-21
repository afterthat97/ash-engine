#include "utilities.h"

vec3 operator * (const vec3& v1, float f);

vec3 operator / (const vec3& v1, float f);

vec4 operator * (const vec4& v1, float f);

vec4 operator / (const vec4& v1, float f);

void dumpVec2(vec2);

void dumpVec3(vec3);

void dumpVec4(vec4);

void dumpMat2(mat2);

void dumpMat3(mat3);

void dumpMat4(mat4);

mat3 rotate(vec3 axis, float angle);

void getClosestPointOfLineLine(vec3 st1, vec3 dir1, vec3 st2, vec3 dir2, vec3& p1, vec3& p2);

void getIntersectionOfLinePlane(vec3 st, vec3 dir, vec3 p, vec3 n, vec3& intersection);

void screenPosToWorldRay(vec2 cursorPos, vec2 windowSize, mat4 proj, mat4 view, vec4& st, vec4& ed);
