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
	float xMax, xMin, yMax, yMin, zMax, zMin;
	uint32_t VAO, VBO, NBO, UBO, EBO;
	vector<float> vertices;
	vector<float> normals;
	vector<float> texCoords;
	vector<uint32_t> indices;
	vector<Material> materials;
	Mesh() {
		VAO = VBO = UBO = EBO = 0;
		xMax = yMax = zMax = -FLT_MAX;
		xMin = yMin = zMin = FLT_MAX;
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