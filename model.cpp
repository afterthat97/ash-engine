#include "model.h"

Model::Model(void * _parent) {
    name = "Untitled Model";
    parent = _parent;
    pos = vec3(FLT_MAX);
}

Model::~Model() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        delete meshes[i];
    for (uint32_t i = 0; i < children.size(); i++)
        delete children[i];
}

void Model::addMesh(Mesh *newMesh) {
    pos = minVec3(pos, newMesh->getPosition());
    newMesh->setParent(this);
    meshes.push_back(newMesh);
}

void Model::addChildren(Model *newModel) {
    pos = minVec3(pos, newModel->getPosition());
    newModel->setParent(this);
    children.push_back(newModel);
}

void Model::removeMesh(Mesh *target) {
    pos = vec3(FLT_MAX);
    for (uint32_t i = 0; i < meshes.size(); i++)
        if (meshes[i] == target)
            meshes.erase(meshes.begin() + i);
    for (uint32_t i = 0; i < meshes.size(); i++)
        pos = minVec3(pos, meshes[i]->getPosition());
}

void Model::removeChildren(Model *target) {
    pos = vec3(FLT_MAX);
    for (uint32_t i = 0; i < children.size(); i++)
        if (children[i] == target)
            children.erase(children.begin() + i);
    for (uint32_t i = 0; i < children.size(); i++)
        pos = minVec3(pos, children[i]->getPosition());
}

void Model::show() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->show();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->show();
}

void Model::hide() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->hide();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->hide();
}

void Model::select() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->select();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->select();
}

void Model::deselect() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->deselect();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->deselect();
}

void Model::render(Shader& shader) {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->render(shader);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->render(shader);
}

void Model::dumpinfo(string tab) {
    printf("%sModel %s with %d meshes and %d children in total.\n", tab.c_str(), name.c_str(), (int) meshes.size(), (int) children.size());
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->dumpinfo(tab + "  ");
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->dumpinfo(tab + "  ");
}

void Model::addTranslation(vec3 delta) {
    pos += delta;
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->addTranslation(delta);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->addTranslation(delta);
}

void Model::addRotation(vec3 eularAngle) {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->addRotation(eularAngle);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->addRotation(eularAngle);
}

void Model::setPosition(vec3 newPos) {
    addTranslation(newPos - pos);
}

vec3 Model::getPosition() {
    return pos;
}

void Model::setParent(void * _parent) {
    parent = _parent;
}

void * Model::getParent() {
    return parent;
}
