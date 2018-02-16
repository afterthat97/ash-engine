#ifndef LIGHT_H
#define LIGHT_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

class Light: public Mesh {
public:
	vec3 color;
	Light(vec3);
};

#endif
