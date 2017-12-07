#include "material.h"

void Material::loadDefault() {
	name = "";
	ambient = vec3(0.2f, 0.2f, 0.2f);
	diffuse = vec3(0.8f, 0.8f, 0.8f);
	specular = vec3(0.1f, 0.1f, 0.1f);
	shininess = 32.0f;
	sharpness = 60.0f;
	density = 1.0f;
}

void Material::bind(Shader& shader) {
	// Use and set shader
	shader.use();
	shader.setVec3("material.ambientRGB", ambient);
	shader.setVec3("material.diffuseRGB", diffuse);
	shader.setVec3("material.specularRGB", specular);
	shader.setInt("material.ambientMap", ambientMap);
	shader.setInt("material.diffuseMap", diffuseMap);
	shader.setInt("material.specularMap", specularMap);
	shader.setFloat("material.shininess", shininess);
	if (ambientMap != 0) {
		shader.setInt("material.hasAmbientMap", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ambientMap);
	} else
		shader.setInt("material.hasAmbientMap", 0);
	if (diffuseMap != 0) {
		shader.setInt("material.hasDiffuseMap", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
	} else
		shader.setInt("material.hasDiffuseMap", 0);
	if (specularMap != 0) {
		shader.setInt("material.hasSpecularMap", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, specularMap);
	} else
		shader.setInt("material.hasSpecularMap", 0);
}

void Material::dumpinfo(string tab) {
//	printf("%sMaterial %s, %lu textures in total.\n",
//		tab.c_str(), name.c_str(), textureIndices.size());
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
//	printf("%s  Emission:", tab.c_str());
//	for (int32_t i = 0; i < 3; i++)
//		printf(" %.2f", emission[i]);
//	printf("\n");
	printf("%s  Shininess: %.2f\n", tab.c_str(), shininess);
	printf("%s  Sharpness: %.2f\n", tab.c_str(), sharpness);
	printf("%s  Density: %.2f\n", tab.c_str(), density);
}
