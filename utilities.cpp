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

string getFormatDate() {
	time_t rawtime;
	time(&rawtime);
	tm *timeinfo = localtime (&rawtime);
	return to_string(timeinfo->tm_year + 1900)
		 + "-" + to_string(timeinfo->tm_mon + 1)
		 + "-" + to_string(timeinfo->tm_mday);
}

string getFormatTime() {
	time_t rawtime;
	time(&rawtime);
	tm *timeinfo = localtime (&rawtime);
	return to_string(timeinfo->tm_hour)
		+ "." + to_string(timeinfo->tm_min)
		+ "." + to_string(timeinfo->tm_sec);
}

vec3 minVec3(const vec3& v1, const vec3& v2) {
	return vec3(min(v1[0], v2[0]), min(v1[1], v2[1]), min(v1[2], v2[2]));
}

vec3 maxVec3(const vec3& v1, const vec3& v2) {
	return vec3(max(v1[0], v2[0]), max(v1[1], v2[1]), max(v1[2], v2[2]));
}
