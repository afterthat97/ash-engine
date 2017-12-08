#ifndef SCENE_H
#define SCENE_H

#include "utilities.h"
#include "shader.h"
#include "model.h"
#include "material.h"

struct Scene {
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	vector<Model> models;
	vector<Material> materials;
	Scene() {
		xMax = yMax = zMax = -FLT_MAX;
		xMin = yMin = zMin = FLT_MAX;
	}
	void LoadFromFile(string);
	void initBO();
	void computeSDF(uint32_t);
	void render(Shader&);
	void renderSDF(Shader&);
	void dumpinfo(string);
};

#endif
