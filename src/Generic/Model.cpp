#include <Generic/Model.h>

Model::Model(QObject * parent): QObject(parent) {
    setObjectName("Untitled");
    visible = true;
    position = QVector3D();
    rotation = QVector3D();
    scaling = QVector3D(1.0f, 1.0f, 1.0f);
}

Model::~Model() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        delete meshes[i];
    for (uint32_t i = 0; i < children.size(); i++)
        delete children[i];
}

// Get properties

bool Model::isVisible() {
    return visible;
}

QVector3D Model::getPosition() {
    return position;
}

QVector3D Model::getRotation() {
    return rotation;
}

QVector3D Model::getScaling() {
    return scaling;
}

vector<Mesh*> Model::getMeshes() {
    return meshes;
}

vector<Model*> Model::getChildren() {
    return children;
}

// Transform functions

void Model::translate(QVector3D delta) {
    setPosition(position + delta);
    positionChanged(position); // Send signals
}

void Model::rotate(QVector3D delta) {
    setRotation(rotation + delta);
    rotationChanged(rotation); // Send signals
}

void Model::scale(QVector3D delta) {
    setScaling(scaling * delta);
    scalingChanged(scaling); // Send signals
}

// Public slots

void Model::setVisible(bool _visible) {
    visible = _visible;
}

void Model::setPosition(QVector3D _position) {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->translate(_position - position);
    for (uint32_t i = 0; i < children.size(); i++)
        children[i]->translate(_position - position);
    position = _position;
}

void Model::setRotation(QVector3D _rotation) {
    for (uint32_t i = 0; i < meshes.size(); i++) {
        meshes[i]->setPosition(_rotation * (meshes[i]->getPosition() - position) + position);
        meshes[i]->rotate(_rotation - rotation);
    }
    for (uint32_t i = 0; i < children.size(); i++) {
        children[i]->setPosition(_rotation * (children[i]->getPosition() - position) + position);
        children[i]->rotate(_rotation - rotation);
    }
    rotation = _rotation;
}

void Model::setScaling(QVector3D _scaling) {
    for (uint32_t i = 0; i < meshes.size(); i++) {
        meshes[i]->setPosition(_scaling * (meshes[i]->getPosition() - position) + position);
        meshes[i]->scale(_scaling / scaling);
    }
    for (uint32_t i = 0; i < children.size(); i++) {
        children[i]->setPosition(_scaling * (meshes[i]->getPosition() - position) + position);
        children[i]->scale(_scaling / scaling);
    }
    scaling = _scaling;
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
