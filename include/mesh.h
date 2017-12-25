#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "material.h"
#include "vertex.h"
#include "shader.h"

class Mesh {
public:
	string name;
	vec3 minv, maxv, lenv;
	uint32_t VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Material> materials;
	Mesh() {
		VAO = VBO = EBO = 0;
		minv = vec3(FLT_MAX);
		maxv = vec3(-FLT_MAX);
		lenv = vec3(0);
	}
	~Mesh();
	void initBO();
	void render(Shader&);
	void dumpinfo(string);
};

#endif
