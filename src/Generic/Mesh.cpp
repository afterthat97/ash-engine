#include <Generic/Mesh.h>
#include <Generic/Model.h>

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

QVector3D Mesh::getLocalPosition() {
    return position;
}

QVector3D Mesh::getLocalRotation() {
    return rotation;
}

QVector3D Mesh::getLocalScaling() {
    return scaling;
}

QMatrix4x4 Mesh::getLocalModelMatrix() {
    QMatrix4x4 model;
    model.translate(position);
    model.rotate(QQuaternion::fromEulerAngles(rotation));
    model.scale(scaling);
    return model;
}

QMatrix4x4 Mesh::getGlobalModelMatrix() {
    if (parent())
        return static_cast<Model*>(parent())->getGlobalModelMatrix() * getLocalModelMatrix();
    else
        return getLocalModelMatrix();
}

QVector3D Mesh::getGlobalPosition() {
    return getGlobalModelMatrix() * getLocalPosition();
}

QVector3D Mesh::getCenterOfMass() {
    QVector3D centerOfMass;
    for (uint32_t i = 0; i < indices.size();) {
        QVector3D centroid;
        float mass = 0;
        if (meshType == Point) {
            centroid = vertices[indices[i]].position;
            mass = 1.0f;
            i += 1;
        } else if (meshType == Line) {
            centroid = (vertices[indices[i]].position +
                        vertices[indices[i + 1]].position) / 2;
            mass = vertices[indices[i]].position.distanceToPoint(vertices[indices[i + 1]].position);
            i += 2;
        } else if (meshType == Triangle) {
            centroid = (vertices[indices[i]].position +
                        vertices[indices[i + 1]].position +
                        vertices[indices[i + 2]].position) / 3;
            mass = QVector3D::crossProduct(vertices[indices[i + 1]].position - vertices[indices[i]].position,
                                             vertices[indices[i + 2]].position - vertices[indices[i]].position).length() / 2.0f;
            i += 3;
        }
        centerOfMass += centroid * mass;
    }
    return centerOfMass / getMass();
}

float Mesh::getMass() {
    float totalMass = 0;
    for (uint32_t i = 0; i < indices.size();) {
        if (meshType == Point) {
            totalMass += 1.0f;
            i += 1;
        } else if (meshType == Line) {
            totalMass += vertices[indices[i]].position.distanceToPoint(vertices[indices[i + 1]].position);
            i += 2;
        } else if (meshType == Triangle) {
            totalMass += QVector3D::crossProduct(vertices[indices[i + 1]].position - vertices[indices[i]].position,
                                                 vertices[indices[i + 2]].position - vertices[indices[i]].position).length() / 2.0f;
            i += 3;
        }
    }
    return totalMass;
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
    setPosition(position + delta);
    positionChanged(position); // Send signals
}

void Mesh::rotate(QQuaternion _rotation) {
    setRotation(_rotation * rotation);
    rotationChanged(rotation); // Send signals
}

void Mesh::rotate(QVector3D _rotation) {
    setRotation(QQuaternion::fromEulerAngles(_rotation) * rotation);
    rotationChanged(rotation); // Send signals
}

void Mesh::scale(QVector3D _scaling) {
    setScaling(scaling * _scaling);
    scalingChanged(scaling); // Send signals
}

// Public slots

void Mesh::setVisible(bool _visible) {
    visible = _visible;
}

void Mesh::setReverseNormal(bool _reverseNormal) {
    reverseNormal = _reverseNormal;
}

void Mesh::resetTransformation() {
    position = QVector3D();
    rotation = QVector3D();
    scaling = QVector3D(1.0f, 1.0f, 1.0f);
}

void Mesh::setPosition(QVector3D newPosition) {
    position = newPosition;
}

void Mesh::setRotation(QQuaternion _rotation) {
    rotation = _rotation.toEulerAngles();
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
