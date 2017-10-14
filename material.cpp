#include "material.h"
#include "glconfig.h"

void Material::loadDefault() {
	name = "";
	textureIndices.clear();
	shininess = 0.0f; sharpness = 60.0f; density = 1.0f;
	ambient[0] = 0.2f; ambient[1] = 0.2f; ambient[2] = 0.2f; ambient[3] = 1.0f;
	diffuse[0] = 0.8f; diffuse[1] = 0.8f; diffuse[2] = 0.8f; diffuse[3] = 1.0f;
	specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f; specular[3] = 1.0f;
	emission[0] = 0.0f; emission[1] = 0.0f; emission[2] = 0.0f; emission[3] = 0.0f;
}

void Material::draw() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	for (uint32_t i = 0; i < textureIndices.size(); i++) {
		//		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIndices[i]);
	}
}

void Material::dumpinfo(string tab) {
	printf("%sMaterial %s, %lu textures in total.\n",
		tab.c_str(), name.c_str(), textureIndices.size());
	printf("%s  Ambient:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", ambient[i]);
	printf("\n");
	printf("%s  Diffuse:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", diffuse[i]);
	printf("\n");
	printf("%s  Specular:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", specular[i]);
	printf("\n");
	printf("%s  Emission:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", emission[i]);
	printf("\n");
	printf("%s  Shininess: %.2f\n", tab.c_str(), shininess);
	printf("%s  Sharpness: %.2f\n", tab.c_str(), sharpness);
	printf("%s  Density: %.2f\n", tab.c_str(), density);
}
