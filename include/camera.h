#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"

struct Camera {
	vec3 pos, dir, rot, up;
	Camera() {
		pos = vec3(0.0f, 0.0f, 0.0f);
		dir = vec3(0.0f, 0.0f, -1.0f);
		rot = vec3(0.0f, 0.0f, 0.0f);
		up = vec3(0.0f, 1.0f, 0.0f);
	}
	void reset();
	void moveForward(float);
	void moveRight(float);
	void moveUp(float);
	void turnLeft(float);
	void lookUp(float);
	mat4 getViewMatrix();
};

#endif
