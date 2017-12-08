#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "utilities.h"

struct Model {
	string name;
	vec3 minv, maxv, lenv;
	vector<Mesh> meshes;
	vector<Model> children;
	Model() {
		minv = vec3(FLT_MAX);
		maxv = vec3(-FLT_MAX);
		lenv = vec3(0);
	}
	void initBO();
	void computeSDF(uint32_t);
	void render(Shader&);
	void renderSDF(Shader&);
	void dumpinfo(string);
};

#endif
