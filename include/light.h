#ifndef LIGHT_H
#define LIGHT_H

#include "utilities.h"
#include "vector3f.h"

struct Light {
	float ambient[4], diffuse[4], specular[4], position[4];
	uint32_t id;
	Light(float x, float y, float z, float direction, uint32_t _id) {
		position[0] = x; position[1] = y; position[2] = z;
		position[3] = direction; id = _id;
		loadDefault();
	}
	void enable();
	void disable();
	void loadDefault();
};

#endif
