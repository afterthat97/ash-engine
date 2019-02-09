#include <Mesh.h>

Mesh::Mesh(QObject * parent): AbstractEntity(0) {
    m_meshType = Triangle;
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
    m_material = 0;
    resetTransformation();
    setParent(parent);
}

Mesh::Mesh(MeshType _meshType, QObject * parent): AbstractEntity(0) {
    m_meshType = _meshType;
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
    m_material = 0;
    resetTransformation();
    setParent(parent);
}

Mesh::Mesh(const Mesh & mesh): AbstractEntity(mesh) {
    m_meshType = mesh.m_meshType;
    m_vertices = mesh.m_vertices;
    m_indices = mesh.m_indices;
    m_material = new Material(*mesh.m_material);
}

Mesh::~Mesh() {
#ifdef _DEBUG
    qDebug() << "Mesh" << this->objectName() << "is destroyed";
#endif
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

// TODO

#include <Model.h>

bool Mesh::isOnlyChild() const {
    if (Model* par = qobject_cast<Model*>(parent()))
        if (par->childModels().size() == 0 && par->childMeshes().size() == 1)
            return true;
    return false;
}

QVector3D Mesh::centerOfMass() const {
    QVector3D centerOfMass;
    float totalMass = 0;
    QMatrix4x4 modelMat = globalModelMatrix();
    for (int i = 0; i < m_indices.size();) {
        QVector3D centroid;
        float mass = 0;
        if (m_meshType == Point) {
            centroid = modelMat * m_vertices[m_indices[i + 0]].position;
            mass = 1.0f;
            i += 1;
        } else if (m_meshType == Line) {
            QVector3D p0 = modelMat * m_vertices[m_indices[i + 0]].position;
            QVector3D p1 = modelMat * m_vertices[m_indices[i + 1]].position;
            centroid = (p0 + p1) / 2;
            mass = p0.distanceToPoint(p1);
            i += 2;
        } else if (m_meshType == Triangle) {
            QVector3D p0 = modelMat * m_vertices[m_indices[i + 0]].position;
            QVector3D p1 = modelMat * m_vertices[m_indices[i + 1]].position;
            QVector3D p2 = modelMat * m_vertices[m_indices[i + 2]].position;
            centroid = (p0 + p1 + p2) / 3;
            mass = QVector3D::crossProduct(p1 - p0, p2 - p0).length() / 2;
            i += 3;
        }
        centerOfMass += centroid * mass;
        totalMass += mass;
    }
    return centerOfMass / totalMass;
}

float Mesh::mass() const {
    float totalMass = 0;
    QMatrix4x4 modelMat = globalModelMatrix();
    for (int i = 0; i < m_indices.size();) {
        if (m_meshType == Point) {
            totalMass += 1.0f;
            i += 1;
        } else if (m_meshType == Line) {
            QVector3D p0 = modelMat * m_vertices[m_indices[i + 0]].position;
            QVector3D p1 = modelMat * m_vertices[m_indices[i + 1]].position;
            totalMass += p0.distanceToPoint(p1);
            i += 2;
        } else if (m_meshType == Triangle) {
            QVector3D p0 = modelMat * m_vertices[m_indices[i + 0]].position;
            QVector3D p1 = modelMat * m_vertices[m_indices[i + 1]].position;
            QVector3D p2 = modelMat * m_vertices[m_indices[i + 2]].position;
            totalMass += QVector3D::crossProduct(p1 - p0, p2 - p0).length() / 2;
            i += 3;
        }
    }
    return totalMass;
}

Mesh::MeshType Mesh::meshType() const {
    return m_meshType;
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

Mesh * Mesh::merge(const Mesh * mesh1, const Mesh * mesh2) {
    if (mesh1 == 0 && mesh2 == 0) return 0;
    if (mesh1 == 0) {
        mesh1 = mesh2;
        mesh2 = 0;
    }

    Mesh* merged = new Mesh;
    merged->setMaterial(new Material);
    merged->m_indices = mesh1->m_indices;

    QMatrix4x4 modelMat1 = mesh1->globalModelMatrix();
    for (int i = 0; i < mesh1->m_vertices.size(); i++) {
        Vertex v = mesh1->m_vertices[i];
        v.position = modelMat1 * v.position;
        v.tangent = modelMat1 * v.tangent;
        v.bitangent = modelMat1 * v.bitangent;
        v.normal = QMatrix4x4(modelMat1.normalMatrix()) * v.normal;
        merged->m_vertices.push_back(v);
    }

    if (mesh2) {
        for (int i = 0; i < mesh2->m_indices.size(); i++)
            merged->m_indices.push_back(mesh2->m_indices[i] + mesh1->m_vertices.size());
        QMatrix4x4 modelMat2 = mesh2->globalModelMatrix();
        for (int i = 0; i < mesh2->m_vertices.size(); i++) {
            Vertex v = mesh2->m_vertices[i];
            v.position = modelMat2 * v.position;
            v.tangent = modelMat2 * v.tangent;
            v.bitangent = modelMat2 * v.bitangent;
            v.normal = QMatrix4x4(modelMat2.normalMatrix()) * v.normal;
            merged->m_vertices.push_back(v);
        }
    }

    return merged;
}

void Mesh::setMeshType(MeshType meshType) {
    if (m_meshType != meshType) {
        m_meshType = meshType;
        meshTypeChanged(m_meshType);
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
#ifdef _DEBUG
        qDebug() << "Material" << m_material->objectName() << "will be replaced by" << material->objectName();
#endif
        delete m_material;
    }
    m_material = material;
    m_material->setParent(this);
    materialChanged(m_material);
#ifdef _DEBUG
    qDebug() << "Material" << material->objectName() << "is assigned to mesh" << this->objectName();
#endif
}

// Protected

void Mesh::childEvent(QChildEvent * e) {
    if (e->added()) {
#ifdef _DEBUG
        qDebug() << "Mesh" << this->objectName() << "received child event (Type: Added)";
#endif
        setMaterial(qobject_cast<Material*>(e->child()));
    } else if (e->removed()) {
#ifdef _DEBUG
        qDebug() << "Mesh" << this->objectName() << "received child event (Type: Removed)";
#endif
        if (e->child() == m_material)
            m_material = 0;
        materialChanged(m_material);
    }
}

QDataStream & operator>>(QDataStream & in, Mesh::MeshType & meshType) {
    qint32 t;
    in >> t;
    if (t == 0)
        meshType = Mesh::Triangle;
    else if (t == 1)
        meshType = Mesh::Line;
    else
        meshType = Mesh::Point;
    return in;
}
