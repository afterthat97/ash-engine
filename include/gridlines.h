#ifndef GRIDLINES_H
#define GRIDLINES_H

#include "utilities.h"
#include "shader.h"

class Gridlines {
private:
	uint32_t VAO, VBO;
	vector<float> vertices;
public:
	vec3 pos;
	Gridlines() {
		VAO = VBO = 0;
	}
	void init(float, float, float);
	void render(Shader&);
};

#endif
