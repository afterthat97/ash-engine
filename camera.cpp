#include "camera.h"

vec3 rotate(vec3 axis, vec3 vec, float angle) {
	float cosa = cos(angle), sina = sin(angle);
	float ux = axis.x, uy = axis.y, uz = axis.z;
	vec3 ans(0.0f, 0.0f, 0.0f);
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

void Camera::reset() {
	Camera();
}

void Camera::moveForward(float steps = 1.0f) {
	pos = pos + dir * steps;
}

void Camera::moveRight(float steps = 1.0f) {
	pos = pos + glm::cross(dir, up) * steps;
}

void Camera::moveUp(float steps = 1.0f) {
	pos = pos + up * steps;
}

void Camera::turnLeft(float angle = 0.04f) {
	dir = rotate(vec3(0, 1, 0), dir, angle);
	up = rotate(vec3(0, 1, 0), up, angle);
}

void Camera::lookUp(float angle = 0.04f) {
	vec3 t = glm::cross(dir, up);
	dir = rotate(t, dir, angle);
	up = rotate(t, up, angle);
}
