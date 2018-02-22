#ifndef LIGHT_H
#define LIGHT_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

class Light : public Mesh {
private:
	vec3 color;
public:
	Light(vec3 color, btDiscreteDynamicsWorld *_dynamicsWorld);
	Light(const Light&);
	void setColor(vec3 _color);
	vec3 getColor();
};

#endif
