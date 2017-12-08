#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "material.h"
#include "shader.h"
#include "sdf.h"

class Mesh {
public:
	string name;
	SDF sdf;
	vec3 minv, maxv, lenv;
	uint32_t VAO, VBO, NBO, TBO, UBO, EBO;
	vector<float> vertices;
	vector<float> normals;
	vector<float> tangents;
	vector<float> texCoords;
	vector<uint32_t> indices;
	vector<Material> materials;
	Mesh() {
		VAO = VBO = NBO = TBO = UBO = EBO = 0;
		minv = vec3(FLT_MAX);
		maxv = vec3(-FLT_MAX);
		lenv = vec3(0);
	}
	~Mesh();
	void initBO();
	void computeSDF(uint32_t);
	vector<float> pointSampling(uint32_t);
	void render(Shader&);
	void renderSDF(Shader&);
	void dumpinfo(string);
};

#endif
