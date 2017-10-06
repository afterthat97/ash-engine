#include "camera.h"

void Camera::reset() {
	Camera();
}

void Camera::moveForward(GLfloat steps = 1.0f) {
	pos = pos + dir * steps;
}

void Camera::moveRight(GLfloat steps = 1.0f) {
	pos = pos + cross(dir, up) * steps;
}

void Camera::moveUp(GLfloat steps = 1.0f) {
	pos = pos + up * steps;
}

void Camera::turnLeft(GLfloat angle = 0.04f) {
	dir = rotate(vector3f(0, 1, 0), dir, angle);
	up = rotate(vector3f(0, 1, 0), up, angle);
}

void Camera::lookUp(GLfloat angle = 0.04f) {
	vector3f t = cross(dir, up);
	dir = rotate(t, dir, angle);
	up = rotate(t, up, angle);
}
