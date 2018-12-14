#include <Generic/Model.h>

Model::Model(): Object() {
    initID();
    name = "Untitled Model";
}

Model::~Model() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        delete meshes[i];
    for (uint32_t i = 0; i < children.size(); i++)
        delete children[i];
}

void Model::addMesh(Mesh * newMesh) {
    if (!newMesh) return;
    newMesh->setParent(this);
    meshes.push_back(newMesh);
}

void Model::addChildren(Model * newModel) {
    if (!newModel) return;
    newModel->setParent(this);
    children.push_back(newModel);
}

vector<Mesh*> Model::getMeshes() {
    return meshes;
}

vector<Model*> Model::getChildren() {
    return children;
}

void Model::initID() {
    id = Allocator::allocateModelID();
}
