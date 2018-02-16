#ifndef SCENE_H
#define SCENE_H

#include "utilities.h"
#include "shader.h"
#include "model.h"
#include "material.h"

struct Scene {
	string name;
	vec3 minv, maxv, lenv;
	vector<Model> models;
	vector<Material> materials;
	Scene() {
		minv = vec3(FLT_MAX);
		maxv = vec3(-FLT_MAX);
		lenv = vec3(0);
	}
	void LoadFromFile(string);
	void initBO();
	void render(Shader&);
	void addToDynamicsWorld(btDiscreteDynamicsWorld*);
	void dumpinfo(string);
};

#endif
