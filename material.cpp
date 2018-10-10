#include "material.h"
#include "glconfig.h"

Material::Material() {
	name = "Untitled Material";
	loadAllDefault();
}

Material::~Material() {
	reportInfo("Material " + name + " has been unloaded.");
}

// Add textures to material
void Material::addTexture(shared_ptr<Texture> newTexture) {
    textures.push_back(newTexture);
}

void Material::loadAllDefault() {
    loadDefaultName();
    loadDefaultAmbientRGB();
    loadDefaultDiffuseRGB();
    loadDefaultSpecularRGB();
    loadDefaultShininess();
    loadDefaultDensity();
    loadDefaultSharpness();
}

// Default arguments
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
    specular = vec3(0.4f, 0.4f, 0.4f);
}

void Material::bind(Shader& shader) {
    // Use and configure shader
    shader.use();
    shader.setVec3("material.ambientRGB", ambient);
    shader.setVec3("material.diffuseRGB", diffuse);
    shader.setVec3("material.specularRGB", specular);
    shader.setFloat("material.shininess", shininess);
    shader.setBool("material.hasDiffuseMap", false);
    shader.setBool("material.hasSpecularMap", false);
    shader.setBool("material.hasNormalMap", false);
    shader.setBool("material.hasParallaxMap", false);

    // Bind texture
    for (uint32_t i = 0; i < textures.size(); i++)
        textures[i]->bind(shader);
}

// Dump details to console
void Material::dumpinfo(string tab) {
    printf("%sMaterial %s with %d textures in total.\n", tab.c_str(), name.c_str(), (int) textures.size());
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
        textures[i]->dumpinfo(tab + "  ");
}
