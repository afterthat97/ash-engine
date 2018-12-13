#include <Generic/Mesh.h>

Mesh::Mesh(MeshType _meshType): Object() {
    initID();
    initName();
    meshType = _meshType;
    visible = true;
    material = new Material;
    position = QVector3D();
    rotation = QQuaternion();
    scaling = QVector3D(1.0f, 1.0f, 1.0f);
}

Mesh::~Mesh() {
    delete material;
}

void Mesh::setType(MeshType _meshType) {
    meshType = _meshType;
}

void Mesh::setVisible(bool _visible) {
    visible = _visible;
}

void Mesh::setPosition(QVector3D newPosition) {
    position = newPosition;
}

void Mesh::setRotation(QVector3D eularAngles) {
    rotation = QQuaternion::fromEulerAngles(eularAngles);
}

void Mesh::setScaling(QVector3D scalingVector) {
    scaling = scalingVector;
}

void Mesh::setVertices(vector<Vertex> newVertices) {
    vertices = newVertices;
}

void Mesh::setIndices(vector<uint32_t> newIndices) {
    indices = newIndices;
}

void Mesh::setMaterial(Material * newMaterial) {
    if (material) delete material;
    material = newMaterial;
    newMaterial->setParent(this);
}

Mesh::MeshType Mesh::getType() {
    return meshType;
}

bool Mesh::isVisible() {
    return visible;
}

QVector3D Mesh::getPosition() {
    return position;
}

QVector3D Mesh::getRotation() {
    return rotation.toEulerAngles();
}

QVector3D Mesh::getScaling() {
    return scaling;
}

vector<Vertex> Mesh::getVertices() {
    return vertices;
}

vector<uint32_t> Mesh::getIndices() {
    return indices;
}

Material * Mesh::getMaterial() {
    return material;
}

void Mesh::translate(QVector3D delta) {
    position = position + delta;
}

void Mesh::rotate(QVector3D eularAngles) {
    rotation = QQuaternion::fromEulerAngles(eularAngles) * rotation;
}

void Mesh::scale(QVector3D scalingVector) {
    scaling = scaling * scalingVector;
}

QMatrix4x4 Mesh::getModelMatrix() {
    QMatrix4x4 model;
    model.translate(position);
    model.rotate(rotation);
    model.scale(scaling);
    return model;
}

void Mesh::initName() {
    name = "Untitled Mesh";
}

void Mesh::initID() {
    id = Allocator::allocateMeshID();
}
