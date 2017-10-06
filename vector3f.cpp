#include "vector3f.h"
#include <cmath>

float dot(const vector3f& a, const vector3f& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float length(const vector3f& a) {
	return sqrt(dot(a, a));
}

vector3f cross(const vector3f& a, const vector3f& b) {
	return vector3f(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

vector3f rotate(vector3f axis, vector3f vec, float angle) {
	float cosa = cos(angle), sina = sin(angle);
	float ux = axis.x, uy = axis.y, uz = axis.z;
	vector3f ans(0, 0, 0);
	ans.x = (ux * ux * (1 - cosa) + cosa) * vec.x;
	ans.x += (ux * uy * (1 - cosa) - sina * uz) * vec.y;
	ans.x += (ux * uz * (1 - cosa) + sina * uy) * vec.z;
	ans.y = (ux * uy * (1 - cosa) + sina * uz) * vec.x;
	ans.y += (uy * uy * (1 - cosa) + cosa) * vec.y;
	ans.y += (uy * uz * (1 - cosa) - sina * ux) * vec.z;
	ans.z = (ux * uz * (1 - cosa) - sina * uy) * vec.x;
	ans.z += (uy * uz * (1 - cosa) + sina * ux) * vec.y;
	ans.z += (uz * uz * (1 - cosa) + cosa) * vec.z;
	return ans;
}

