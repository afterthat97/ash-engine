#ifndef SDF_H
#define SDF_H

#include "utilities.h"
#include "shader.h"

class SDF {
private:
	uint32_t VAO, VBO, CBO, nx, ny, nz;
	float ***distance, delta;
	vec3 ***gradient;
public:
	SDF() {
		VAO = VBO = 0;
		distance = NULL;
	}
	~SDF();
	vector<float> vertices, colors;
	void computeDistanceField(const vector<float>&, uint32_t);
	void computeGradientVector();
	void initBO();
	void render(Shader&);
};

#endif