#ifndef GRIDLINES_H
#define GRIDLINES_H

#include "utilities.h"
#include "shader.h"

class Gridlines {
private:
	uint32_t VAO, VBO;
	vector<float> vertices;
public:
	mat4 model;
	vec3 pos;
	Gridlines() {
		VAO = VBO = 0;
		model = mat4(1.0);
	}
	void init(float, float, float);
	void render(Shader&);
};

#endif
