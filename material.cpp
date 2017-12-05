#include "material.h"
#include "glconfig.h"

void Material::loadDefault() {
	name = "";
	textureIndices.clear();
	shininess = 32.0f; sharpness = 60.0f; density = 1.0f;
	ambient = vec3(0.2f, 0.2f, 0.2f);
	diffuse = vec3(0.8f, 0.8f, 0.8f);
	specular = vec3(0.1f, 0.1f, 0.1f);
	emission = vec3(0.0f, 0.0f, 0.0f);
}

void Material::bind(Shader& shader) {
	shader.use();
	shader.setInt("texture0", 0);
	shader.setVec3("material.ambient", ambient);
	shader.setVec3("material.diffuse", diffuse);
	shader.setVec3("material.specular", specular);
	shader.setFloat("material.shininess", shininess);
	for (uint32_t i = 0; i < 1; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIndices[i]);
	}
}

void Material::dumpinfo(string tab) {
	printf("%sMaterial %s, %lu textures in total.\n",
		tab.c_str(), name.c_str(), textureIndices.size());
	printf("%s  Ambient:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", ambient[i]);
	printf("\n");
	printf("%s  Diffuse:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", diffuse[i]);
	printf("\n");
	printf("%s  Specular:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", specular[i]);
	printf("\n");
	printf("%s  Emission:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", emission[i]);
	printf("\n");
	printf("%s  Shininess: %.2f\n", tab.c_str(), shininess);
	printf("%s  Sharpness: %.2f\n", tab.c_str(), sharpness);
	printf("%s  Density: %.2f\n", tab.c_str(), density);
}
