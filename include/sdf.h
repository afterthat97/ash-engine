#ifndef SDF_H
#define SDF_H

#include "utilities.h"
#include "shader.h"

class SDF {
private:
	uint32_t VAO, VBO, CBO, nx, ny, nz;
	float ***distance, delta;
	vec3 ***gradient, minv, maxv, len;
public:
	SDF() {
		minv = vec3(FLT_MAX);
		maxv = vec3(-FLT_MAX);
		len = vec3(0);
		VAO = VBO = CBO = nx = ny = nz = 0;
		distance = NULL;
		gradient = NULL;
	}
	~SDF();
	vector<float> vertices, colors;
	void loadFromFile(const string&);
	void computeDistanceField(const vector<float>&, uint32_t);
	void computeGradientVector();
	void initBO();
	void render(Shader&);
};

#endif
