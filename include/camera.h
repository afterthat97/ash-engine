#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"
#include "vector3f.h"

struct Camera {
	vector3f pos, dir, rot, up;
	Camera() {
		pos = vector3f(0, 100, 100);
		dir = vector3f(0, -1, -1);
		rot = vector3f(0, 0, 0);
		up = vector3f(0, 1, -1);
	}
	void reset();
	void moveForward(GLfloat);
	void moveRight(GLfloat);
	void moveUp(GLfloat);
	void turnLeft(GLfloat);
	void lookUp(GLfloat);
};

#endif
