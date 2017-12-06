#ifndef SCENE_H
#define SCENE_H

#include "shader.h"
#include "model.h"
#include "material.h"
#include "utilities.h"

struct Scene {
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	vector<Model> models;
	vector<Material> materials;
	Scene() {
		xMax = yMax = zMax = -FLT_MAX;
		xMin = yMin = zMin = FLT_MAX;
	}
	void initBO();
	void computeSDF(uint32_t);
	void render(Shader&);
	void renderSDF(Shader&);
	void dumpinfo(string);
};

#endif
