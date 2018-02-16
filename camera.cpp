#include "camera.h"
#include "extmath.h"

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
	dir = rotate(vec3(0, 1, 0), angle) * dir;
	up = rotate(vec3(0, 1, 0), angle) * up;
}

void Camera::lookUp(float angle = 0.04f) {
	vec3 t = glm::cross(dir, up);
	dir = rotate(t, angle) * dir;
	up = rotate(t, angle) * up;
}

mat4 Camera::getViewMatrix() {
	return glm::lookAt(pos, pos + dir, up);
}
