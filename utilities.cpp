#include "utilities.h"

void throwError(string type, string name, string msg) {
	throw "ERROR: Failed to " + type + " " + name + ": " + msg;
}

void reportInfo(string msg) {
	cout << "INFO: " << msg << endl;
}

void reportWarning(string msg) {
	cout << "WARNING: " << msg << endl;
}

vec3 minVec3(const vec3& v1, const vec3& v2) {
	return vec3(min(v1[0], v2[0]), min(v1[1], v2[1]), min(v1[2], v2[2]));
}

vec3 maxVec3(const vec3& v1, const vec3& v2) {
	return vec3(max(v1[0], v2[0]), max(v1[1], v2[1]), max(v1[2], v2[2]));
}
