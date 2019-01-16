#include <Core/Mesh.h>
#include <Core/Model.h>
#include <Core/Material.h>

Mesh::Mesh(MeshType _meshType, QObject * parent): QObject(parent) {
    m_meshType = _meshType;
    m_visible = true;
    m_material = 0;
    resetTransformation();
}

// Transform functions

void Mesh::translate(QVector3D delta) {
    setPosition(m_position + delta);
}

void Mesh::rotate(QQuaternion _rotation) {
    setRotation(_rotation * m_rotation);
}

void Mesh::rotate(QVector3D _rotation) {
    setRotation(QQuaternion::fromEulerAngles(_rotation) * m_rotation);
}

void Mesh::scale(QVector3D _scaling) {
    setScaling(m_scaling * _scaling);
}

// Dump info

void Mesh::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Mesh: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Type: " <<
        (m_meshType == Triangle ? "Triangle" : (m_meshType == Line ? "Line" : "Point"));
    qDebug().nospace() << tab(l + 1) << "Visible: " << m_visible;
    qDebug().nospace() << tab(l + 1) << "Position: " << m_position;
    qDebug().nospace() << tab(l + 1) << "Rotation: " << m_rotation;
    qDebug().nospace() << tab(l + 1) << "Scaling:  " << m_scaling;
    qDebug("%s%d vertices, %d indices, %d material",
           tab(l + 1), m_vertices.size(), m_indices.size(), m_material != 0);
}

void Mesh::dumpObjectTree(int l) {
    dumpObjectInfo(l);
    if (m_material)
        m_material->dumpObjectTree(l + 1);
}

// Others

QVector3D Mesh::centerOfMass() const {
    QVector3D centerOfMass;
    for (int i = 0; i < m_indices.size();) {
        QVector3D centroid;
        float mass = 0;
        if (m_meshType == Point) {
            centroid = m_vertices[m_indices[i]].position;
            mass = 1.0f;
            i += 1;
        } else if (m_meshType == Line) {
            centroid = (m_vertices[m_indices[i]].position +
                        m_vertices[m_indices[i + 1]].position) / 2;
            mass = m_vertices[m_indices[i]].position.distanceToPoint(m_vertices[m_indices[i + 1]].position);
            i += 2;
        } else if (m_meshType == Triangle) {
            centroid = (m_vertices[m_indices[i]].position +
                        m_vertices[m_indices[i + 1]].position +
                        m_vertices[m_indices[i + 2]].position) / 3;
            mass = QVector3D::crossProduct(m_vertices[m_indices[i + 1]].position - m_vertices[m_indices[i]].position,
                                           m_vertices[m_indices[i + 2]].position - m_vertices[m_indices[i]].position).length() / 2.0f;
            i += 3;
        }
        centerOfMass += centroid * mass;
    }
    return centerOfMass / mass();
}

float Mesh::mass() const {
    float totalMass = 0;
    for (int i = 0; i < m_indices.size();) {
        if (m_meshType == Point) {
            totalMass += 1.0f;
            i += 1;
        } else if (m_meshType == Line) {
            totalMass += m_vertices[m_indices[i]].position.distanceToPoint(m_vertices[m_indices[i + 1]].position);
            i += 2;
        } else if (m_meshType == Triangle) {
            totalMass += QVector3D::crossProduct(m_vertices[m_indices[i + 1]].position - m_vertices[m_indices[i]].position,
                                                 m_vertices[m_indices[i + 2]].position - m_vertices[m_indices[i]].position).length() / 2.0f;
            i += 3;
        }
    }
    return totalMass;
}

// Get properties

bool Mesh::visible() const {
    return m_visible;
}

Mesh::MeshType Mesh::meshType() const {
    return m_meshType;
}

QVector3D Mesh::localPosition() const {
    return m_position;
}

QVector3D Mesh::globalPosition() const {
    return globalModelMatrix() * localPosition();
}

QVector3D Mesh::localRotation() const {
    return m_rotation;
}

QVector3D Mesh::localScaling() const {
    return m_scaling;
}

QMatrix4x4 Mesh::localModelMatrix() const {
    QMatrix4x4 model;
    model.translate(m_position);
    model.rotate(QQuaternion::fromEulerAngles(m_rotation));
    model.scale(m_scaling);
    return model;
}

QMatrix4x4 Mesh::globalModelMatrix() const {
    if (Model* par = qobject_cast<Model*>(parent()))
        return par->globalModelMatrix() * localModelMatrix();
    else
        return localModelMatrix();
}

const QVector<Vertex>& Mesh::vertices() const {
    return m_vertices;
}

const QVector<uint32_t>& Mesh::indices() const {
    return m_indices;
}

Material * Mesh::material() const{
    return m_material;
}

// Public slots

void Mesh::resetTransformation() {
    setPosition(QVector3D());
    setRotation(QVector3D());
    setScaling(QVector3D(1.0f, 1.0f, 1.0f));
}

void Mesh::setVisible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        visibleChanged(m_visible);
    }
}

void Mesh::setMeshType(MeshType meshType) {
    if (m_meshType != meshType) {
        m_meshType = meshType;
        meshTypeChanged(m_meshType);
    }
}

void Mesh::setPosition(QVector3D position) {
    if (!qFuzzyCompare(m_position, position)) {
        m_position = position;
        positionChanged(m_position);
    }
}

void Mesh::setRotation(QQuaternion rotation) {
    if (!qFuzzyCompare(m_rotation, rotation.toEulerAngles())) {
        m_rotation = rotation.toEulerAngles();
        rotationChanged(m_rotation);
    }
}

void Mesh::setRotation(QVector3D rotation) {
    if (!qFuzzyCompare(m_rotation, rotation)) {
        m_rotation = rotation;
        rotationChanged(m_rotation);
    }
}

void Mesh::setScaling(QVector3D scaling) {
    if (!qFuzzyCompare(m_scaling, scaling)) {
        m_scaling = scaling;
        scalingChanged(m_scaling);
    }
}

void Mesh::setGeometry(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices) {
    if (m_vertices != vertices || m_indices != indices) {
        m_vertices = vertices;
        m_indices = indices;
        geometryChanged(m_vertices, m_indices);
    }
}

void Mesh::setMaterial(Material * material) {
    if (material == 0 || m_material == material) return;
    if (m_material) {
        qWarning("Mesh::setMaterial: Existed material will be overrided.");
        delete m_material;
    }
    m_material = material;
    m_material->setParent(this);
    materialChanged(m_material);
}

// Protected

void Mesh::childEvent(QChildEvent * e) {
    if (e->added()) {
        setMaterial(static_cast<Material*>(e->child()));
    } else if (e->removed()) {
        if (e->child() == m_material)
            m_material = 0;
        materialChanged(m_material);
    }
}
