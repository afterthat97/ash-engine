#ifndef VERTEX_H
#define VERTEX_H

#include "utilities.h"

class Vertex {
public:
	vec3 position, normal, tangent;
	vec2 texCoord;
	Vertex() {
		position = normal = tangent = vec3(0.0f);
		texCoord = vec2(0.0f);
	}
};

#endif
