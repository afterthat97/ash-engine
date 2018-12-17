#include <Generic/Model.h>
#include <Generic/Scene.h>

Model::Model(QObject * parent): QObject(parent) {
    setObjectName("Untitled");
    visible = true;
    topLevel = false;
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

bool Model::isTopLevel() {
    return topLevel;
}

QVector3D Model::getLocalPosition() {
    return position;
}

QVector3D Model::getLocalRotation() {
    return rotation;
}

QVector3D Model::getLocalScaling() {
    return scaling;
}

QMatrix4x4 Model::getLocalModelMatrix() {
    QMatrix4x4 model;
    model.translate(position);
    model.rotate(QQuaternion::fromEulerAngles(rotation));
    model.scale(scaling);
    return model;
}

QMatrix4x4 Model::getGlobalModelMatrix() {
    if (topLevel || parent() == 0)
        return getLocalModelMatrix();
    else
        return static_cast<Model*>(parent())->getGlobalModelMatrix() * getLocalModelMatrix();
}

QVector3D Model::getGlobalPosition() {
    if (topLevel || parent() == 0)
        return getLocalPosition();
    else
        return getGlobalModelMatrix() * getLocalPosition();
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

void Model::rotate(QQuaternion _rotation) {
    setRotation(_rotation * rotation);
    rotationChanged(rotation); // Send signals
}

void Model::rotate(QVector3D _rotation) {
    setRotation(QQuaternion::fromEulerAngles(_rotation) * rotation);
    rotationChanged(rotation); // Send signals
}

void Model::scale(QVector3D _scaling) {
    setScaling(scaling * _scaling);
    scalingChanged(scaling); // Send signals
}

// Public slots

void Model::setVisible(bool _visible) {
    visible = _visible;
}

void Model::setTopLevel(bool _topLevel) {
    topLevel = _topLevel;
}

void Model::resetTransformation() {
    position = QVector3D();
    rotation = QVector3D();
    scaling = QVector3D(1.0f, 1.0f, 1.0f);
}

void Model::resetChildrenTransformation() {
    for (uint32_t i = 0; i < meshes.size(); i++)
        meshes[i]->resetTransformation();
    for (uint32_t i = 0; i < children.size(); i++) {
        children[i]->resetTransformation();
        children[i]->resetChildrenTransformation();
    }
}

void Model::setPosition(QVector3D _position) {
    position = _position;
}

void Model::setRotation(QQuaternion _rotation) {
    rotation = _rotation.toEulerAngles();
}

void Model::setRotation(QVector3D _rotation) {
    rotation = _rotation;
}

void Model::setScaling(QVector3D _scaling) {
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
