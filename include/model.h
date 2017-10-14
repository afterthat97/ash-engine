#ifndef MODEL_H
#define MODEL_H

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
	void draw();
	void dumpinfo(string);
};

#endif
