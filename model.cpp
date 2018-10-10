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

// Add mesh
void Model::addMesh(Mesh *newMesh) {
    pos = minVec3(pos, newMesh->getPosition());
    newMesh->setParent(this);
    meshes.push_back(newMesh);
}

// Add child model
void Model::addChildren(Model *newModel) {
    pos = minVec3(pos, newModel->getPosition());
    newModel->setParent(this);
    children.push_back(newModel);
}

// Count all meshes belong to this model
uint32_t Model::getMeshNum() {
    uint32_t tot = meshes.size();
    for (uint32_t i = 0; i < children.size(); i++)
        tot += children[i]->getMeshNum();
    return tot;
}

// Remove mesh
void Model::removeMesh(Mesh *target) {
    pos = vec3(FLT_MAX);
    for (uint32_t i = 0; i < meshes.size(); i++)
        if (meshes[i] == target)
            meshes.erase(meshes.begin() + i);
    for (uint32_t i = 0; i < meshes.size(); i++)
        pos = minVec3(pos, meshes[i]->getPosition());
}

// Remove chile model
void Model::removeChildren(Model *target) {
    pos = vec3(FLT_MAX);
    for (uint32_t i = 0; i < children.size(); i++)
        if (children[i] == target)
            children.erase(children.begin() + i);
    for (uint32_t i = 0; i < children.size(); i++)
        pos = minVec3(pos, children[i]->getPosition());
}

// Recycle memory
void Model::recycle() {
    for (uint32_t i = 0; i < children.size(); i++)
        if (children[i]->getMeshNum() == 0) {
            delete children[i];
            children.erase(children.begin() + i);
            i--;
        }
}

// Show the model on screen
void Model::show() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->show();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->show();
}

// Hide the model
void Model::hide() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->hide();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->hide();
}

// Select the whole model
void Model::select() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->select();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->select();
}

// Deselect the whole model
void Model::deselect() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->deselect();
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->deselect();
}

// Render
void Model::render(Shader& shader) {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->render(shader);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->render(shader);
}

// Dump details to console
void Model::dumpinfo(string tab) {
    printf("%sModel %s with %d meshes and %d children in total.\n", tab.c_str(), name.c_str(), (int) meshes.size(), (int) children.size());
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->dumpinfo(tab + "  ");
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->dumpinfo(tab + "  ");
}

// Apply translation to the model
void Model::addTranslation(vec3 delta) {
    pos += delta;
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->addTranslation(delta);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->addTranslation(delta);
}

// Apply rotation to the model
void Model::addRotation(vec3 eularAngle) {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->addRotation(eularAngle);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->addRotation(eularAngle);
}

// Set the postion of the model
void Model::setPosition(vec3 newPos) {
    addTranslation(newPos - pos);
}

// Get the position of the model
vec3 Model::getPosition() {
    return pos;
}

// Get the size of the model
vec3 Model::getSize() {
    vec3 maxv = vec3(-FLT_MAX);
    for (uint32_t i = 0; i < meshes.size(); i++)
        maxv = maxVec3(maxv, meshes[i]->getPosition() + meshes[i]->getSize());
    for (uint32_t i = 0; i < children.size(); i++)
        maxv = maxVec3(maxv, children[i]->getPosition() + children[i]->getSize());
    return maxv - pos;
}

// Set the parent model
void Model::setParent(void * _parent) {
    parent = _parent;
}

// Get the parent model
void * Model::getParent() {
    return parent;
}
