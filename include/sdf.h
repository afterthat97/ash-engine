#ifndef SDF_H
#define SDF_H

#include "utilities.h"

class SDF {
private:
	uint32_t VAO, VBO;
public:
	SDF(const vector<float> vertices, float delta) {
		VAO = VBO = 0;
		compute(vertices, delta);
	}
	SDF() {
		VAO = VBO = 0;
	}
	~SDF();
	void compute(const vector<float>, float);
	void render();
};

#endif