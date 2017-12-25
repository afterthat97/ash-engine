#include "material.h"
#include "glconfig.h"

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
	diffuse = vec3(1.0f, 1.0f, 1.0f);
}

void Material::loadDefaultSpecularRGB() {
	specular = vec3(0.5f, 0.5f, 0.5f);
}

void Material::bind(Shader& shader) {
	// Use and set shader
	shader.use();
	shader.setVec3("material.ambientRGB", ambient);
	shader.setVec3("material.diffuseRGB", diffuse);
	shader.setVec3("material.specularRGB", specular);
	shader.setFloat("material.shininess", shininess);
	shader.setInt("material.hasAmbientMap", 0);
	shader.setInt("material.hasDiffuseMap", 0);
	shader.setInt("material.hasSpecularMap", 0);
	shader.setInt("material.hasNormalMap", 0);

	// Bind texture
	if (enableTexture)
		for (uint32_t i = 0; i < textures.size(); i++)
			textures[i].bind(shader);
}

void Material::dumpinfo(string tab) {
	printf("%sMaterial %s, %lu textures in total.\n",
		tab.c_str(), name.c_str(), textures.size());
	printf("%s  Ambient RGB:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", ambient[i]); printf("\n");
	printf("%s  Diffuse RGB:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", diffuse[i]); printf("\n");
	printf("%s  Specular RGB:", tab.c_str());
	for (int32_t i = 0; i < 3; i++)
		printf(" %.2f", specular[i]); printf("\n");
	printf("%s  Shininess: %.2f\n", tab.c_str(), shininess);
	printf("%s  Sharpness: %.2f\n", tab.c_str(), sharpness);
	printf("%s  Density: %.2f\n", tab.c_str(), density);
	for (uint32_t i = 0; i < textures.size(); i++)
		textures[i].dumpinfo(tab + "  ");
}
