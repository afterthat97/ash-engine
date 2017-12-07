#include "material.h"

void Material::loadAllDefault() {
	loadDefaultName();
	loadDefaultAmbientRGB();
	loadDefaultDiffuseRGB();
	loadDefaultSpecularRGB();
	loadDefaultShininess();
	loadDefaultDensity();
	loadDefaultSharpness();
}

void Material::loadDefaultName() {
	name = "Untitled";
}

void Material::loadDefaultShininess() {
	shininess = 32.0f;
}

void Material::loadDefaultSharpness() {
	sharpness = 60.0f;
}

void Material::loadDefaultDensity() {
	density = 1.0f;
}

void Material::loadDefaultAmbientRGB() {
	ambient = vec3(0.2f, 0.2f, 0.2f);
}

void Material::loadDefaultDiffuseRGB() {
	diffuse = vec3(0.8f, 0.8f, 0.8f);
}

void Material::loadDefaultSpecularRGB() {
	specular = vec3(0.1f, 0.1f, 0.1f);
}

void Material::apply(Shader& shader) {
	// Use and set shader
	shader.use();
	shader.setVec3("material.ambientRGB", ambient);
	shader.setVec3("material.diffuseRGB", diffuse);
	shader.setVec3("material.specularRGB", specular);
	shader.setFloat("material.shininess", shininess);
	shader.setInt("material.ambientMap", 0);
	shader.setInt("material.diffuseMap", 1);
	shader.setInt("material.specularMap", 2);
	shader.setInt("material.normalMap", 3);
	shader.setInt("material.hasAmbientMap", ambientMap ? 1 : 0);
	shader.setInt("material.hasDiffuseMap", diffuseMap ? 1 : 0);
	shader.setInt("material.hasSpecularMap", specularMap ? 1 : 0);
	shader.setInt("material.hasNormalMap", normalMap ? 1 : 0);
}

void Material::bind(Shader& shader) {
	// Use and set shader
	shader.use();

	// Bind ambient map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ambientMap);

	// Bind diffuse map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	// Bind specular map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	// Bind normal map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);
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
