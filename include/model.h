#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "utilities.h"

struct Model {
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	vector<Mesh> meshes;
	vector<Model> children;
	Model() {
		xMax = yMax = zMax = -FLT_MAX;
		xMin = yMin = zMin = FLT_MAX;
	}
	void init();
	void render(Shader&);
	void dumpinfo(string);
};

#endif
