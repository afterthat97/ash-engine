#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include "material.h"
#include "utilities.h"

struct Scene {
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	vector<Model> models;
	vector<Material> materials;
	Scene(string filename) {
		xMax = yMax = zMax = -FLT_MAX;
		xMin = yMin = zMin = FLT_MAX;
		loadFromFile(filename);
	}
	void loadFromFile(string);
	void draw();
	void dumpinfo(string);
};

#endif
