#ifndef AXIS_H
#define AXIS_H

#include "utilities.h"
#include "shader.h"

class Axis {
private:
	uint32_t VAO, VBO, EBO;
	vector<float> vertices;
	vector<uint32_t> indices;
public:
	vec3 pos;
	Axis() {
		VAO = VBO = EBO = 0;
	}
	void init(float);
	void render(Shader&);
};

#endif
