#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "face.h"
#include "material.h"

struct Mesh {
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec3> texCoords;
	vector<Face> faces;
	vector<Material> materials;
	Mesh() {
		xMax = yMax = zMax = -FLT_MAX;
		xMin = yMin = zMin = FLT_MAX;
	}
	void draw();
	void dumpinfo(string);
};

#endif
