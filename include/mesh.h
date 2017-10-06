#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "face.h"
#include "vector3f.h"
#include "material.h"

struct Mesh {
	string name;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	vector<vector3f> vertices;
	vector<vector3f> normals;
	vector<vector3f> texCoords;
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
