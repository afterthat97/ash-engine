#ifndef VECTOR3F_H
#define VECTOR3F_H

struct vector3f {
	float x, y, z;
	vector3f() { x = y = z = 0; }
	vector3f(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
	vector3f operator + (const vector3f& vec) {
		return vector3f(x + vec.x, y + vec.y, z + vec.z);
	}
	vector3f operator - (const vector3f& vec) {
		return vector3f(x - vec.x, y - vec.y, z - vec.z);
	}
	vector3f operator * (const float k) {
		return vector3f(x * k, y * k, z * k);
	}
	vector3f operator / (const vector3f& vec) {
		return vector3f(x / vec.x, y / vec.y, z / vec.z);
	}
};

float dot(const vector3f& a, const vector3f& b);
float length(const vector3f& a);
vector3f cross(const vector3f& a, const vector3f& b);
vector3f rotate(vector3f axis, vector3f vec, float angle);

#endif