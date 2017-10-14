#include "mesh.h"

void Mesh::draw() {
	glColor3f(1.0f, 1.0f, 1.0f);
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].draw();
	for (uint32_t i = 0; i < faces.size(); i++) {
		glBegin(GL_POLYGON);
		for (uint32_t j = 0; j < faces[i].indices.size(); j++) {
			uint32_t idx = faces[i].indices[j];
			if (normals.size() > 0)
				glNormal3f(normals[idx].x, normals[idx].y, normals[idx].z);
			if (texCoords.size() > 0)
				glTexCoord2f(texCoords[idx].x, texCoords[idx].y);
			else
				glColor3f(0.5f, 0.5f, 0.5f);
			if (vertices.size() > 0)
				glVertex3f(vertices[idx].x, vertices[idx].y, vertices[idx].z);
		}
		glEnd();
	}
}

void Mesh::dumpinfo(string tab) {
	printf("%sMesh %s:\n", tab.c_str(), name.c_str());
	printf("%s  Vertices: %lu\n", tab.c_str(), vertices.size());
	printf("%s  Normals: %lu\n", tab.c_str(), normals.size());
	printf("%s  Texture Coords: %lu\n", tab.c_str(), texCoords.size());
	printf("%s  Faces: %lu\n", tab.c_str(), faces.size());
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].dumpinfo(tab + "  ");
}
