#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"
#include "vector3f.h"

struct Camera {
	vector3f pos, dir, rot, up;
	Camera() {
		pos = vector3f(0.0f, 100.0f, 0.0f);
		dir = vector3f(0.0f, 0.0f, -1.0f);
		rot = vector3f(0.0f, 0.0f, 0.0f);
		up = vector3f(0.0f, 1.0f, 0.0f);
	}
	void reset();
	void moveForward(GLfloat);
	void moveRight(GLfloat);
	void moveUp(GLfloat);
	void turnLeft(GLfloat);
	void lookUp(GLfloat);
};

#endif
