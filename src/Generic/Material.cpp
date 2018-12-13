#include <Generic/Material.h>

Material::Material(): Object() {
    initID();
    initName();
    ambientColor = QVector3D(0.2f, 0.2f, 0.2f);
    diffuseColor = QVector3D(0.8f, 0.8f, 0.8f);
    specularColor = QVector3D(0.5f, 0.5f, 0.5f);
    shininess = 32.0f;
}

Material::~Material() {
    for (uint32_t i = 0; i < textures.size(); i++)
        delete textures[i];
}

void Material::addTexture(Texture * newTexture) {
    if (!newTexture) return;
    textures.push_back(newTexture);
    newTexture->setParent(this);
}

void Material::setAmbientColor(QVector3D color) {
    ambientColor = color;
}

void Material::setDiffuseColor(QVector3D color) {
    diffuseColor = color;
}

void Material::setSpecularColor(QVector3D color) {
    specularColor = color;
}

void Material::setShininess(float value) {
    shininess = value;
}

vector<Texture*> Material::getTextures() {
    return textures;
}

QVector3D Material::getAmbientColor() {
    return ambientColor;
}

QVector3D Material::getDiffuseColor() {
    return diffuseColor;
}

QVector3D Material::getSpecularColor() {
    return specularColor;
}

float Material::getShininess() {
    return shininess;
}

void Material::initName() {
    name = "Untitled Material";
}

void Material::initID() {
    id = Allocator::allocateMaterialID();
}
