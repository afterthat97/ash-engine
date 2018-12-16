#include <Generic/Mesh.h>

Mesh::Mesh(MeshType _meshType, QObject * parent): QObject(parent) {
    id = Allocator::allocateMeshID();
    setObjectName("Untitled");
    meshType = _meshType;
    visible = true;
    reverseNormal = false;
    material = new Material(this);
    position = QVector3D();
    rotation = QVector3D();
    scaling = QVector3D(1.0f, 1.0f, 1.0f);
}

Mesh::~Mesh() {
    delete material;
}

// Get properties

uint32_t Mesh::getID() {
    return id;
}

Mesh::MeshType Mesh::getType() {
    return meshType;
}

bool Mesh::isVisible() {
    return visible;
}

bool Mesh::isNormalReversed() {
    return reverseNormal;
}

QVector3D Mesh::getPosition() {
    return position;
}

QVector3D Mesh::getRotation() {
    return rotation;
}

QVector3D Mesh::getScaling() {
    return scaling;
}

QMatrix4x4 Mesh::getModelMatrix() {
    QMatrix4x4 model;
    model.translate(position);
    model.rotate(QQuaternion::fromEulerAngles(rotation));
    model.scale(scaling);
    return model;
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

// Transform functions

void Mesh::translate(QVector3D delta) {
    position = position + delta;
    positionChanged(position); // Send signals
}

void Mesh::rotate(QVector3D _rotation) {
    rotation = _rotation + rotation;
    rotationChanged(rotation); // Send signals
}

void Mesh::scale(QVector3D _scaling) {
    scaling = scaling * _scaling;
    scalingChanged(scaling); // Send signals
}

// Public slots

void Mesh::setVisible(bool _visible) {
    visible = _visible;
}

void Mesh::setReverseNormal(bool _reverseNormal) {
    reverseNormal = _reverseNormal;
}

void Mesh::setPosition(QVector3D newPosition) {
    position = newPosition;
}

void Mesh::setRotation(QVector3D _rotation) {
    rotation = _rotation;
}

void Mesh::setScaling(QVector3D scalingVector) {
    scaling = scalingVector;
}

void Mesh::setVertices(const vector<Vertex>& _vertices) {
    vertices = _vertices;
}

void Mesh::setIndices(const vector<uint32_t>& _indices) {
    indices = _indices;
}

void Mesh::setMaterial(Material * newMaterial) {
    if (material) delete material;
    material = newMaterial;
    newMaterial->setParent(this);
}
