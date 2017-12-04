#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "face.h"
#include "material.h"

class Mesh {
public:
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	uint32_t VAO, VBO, UBO, EBO;
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
	void init();
	void render();
	void dumpinfo(string);
};

#endif
