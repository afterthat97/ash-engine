#include <Model.h>

Model::Model(QObject * parent): AbstractEntity(0) {
    setObjectName("Untitled model");
    setParent(parent);
}

Model::Model(const Model & model): AbstractEntity(model) {
    for (int i = 0; i < model.m_childMeshes.size(); i++)
        addChildMesh(new Mesh(*model.m_childMeshes[i]));
    for (int i = 0; i < model.m_childModels.size(); i++)
        addChildModel(new Model(*model.m_childModels[i]));
}

Model::~Model() {
    if (log_level >= LOG_LEVEL_INFO)
        dout << "Model" << this->objectName() << "is destroyed";
}

bool Model::addChildMesh(Mesh * mesh) {
    if (!mesh || m_childMeshes.contains(mesh))
        return false;

    m_childMeshes.push_back(mesh);
    mesh->setParent(this);
    childMeshAdded(mesh);

    if (log_level >= LOG_LEVEL_INFO)
        dout << "Mesh" << mesh->objectName() << "is added to model" << this->objectName() << "as child";

    return true;
}

bool Model::addChildModel(Model * model) {
    if (!model || m_childModels.contains(model))
        return false;

    m_childModels.push_back(model);
    model->setParent(this);
    childModelAdded(model);

    if (log_level >= LOG_LEVEL_INFO)
        dout << "Model" << model->objectName() << "is added to model" << this->objectName() << "as child";

    return true;
}

bool Model::removeChildMesh(QObject * mesh, bool recursive) {
    for (int i = 0; i < m_childMeshes.size(); i++)
        if (m_childMeshes[i] == mesh) {
            m_childMeshes.erase(m_childMeshes.begin() + i);
            childMeshRemoved(mesh);
            if (log_level >= LOG_LEVEL_INFO)
                dout << "Child mesh" << mesh->objectName() << "is removed from model" << this->objectName();
            if (m_childMeshes.size() == 0 && m_childModels.size() == 0)
                delete this;
            return true;
        }
    if (!recursive) return false;
    for (int i = 0; i < m_childModels.size(); i++)
        if (m_childModels[i]->removeChildMesh(mesh, recursive))
            return true;
    return false;
}

bool Model::removeChildModel(QObject * model, bool recursive) {
    for (int i = 0; i < m_childModels.size(); i++)
        if (m_childModels[i] == model) {
            m_childModels.erase(m_childModels.begin() + i);
            childModelRemoved(model);
            if (log_level >= LOG_LEVEL_INFO)
                dout << "Child model" << model->objectName() << "is removed from model" << this->objectName();
            if (m_childMeshes.size() == 0 && m_childModels.size() == 0)
                delete this;
            return true;
        }
    if (!recursive) return false;
    for (int i = 0; i < m_childModels.size(); i++)
        if (m_childModels[i]->removeChildModel(model, recursive))
            return true;
    return false;
}

// Dump info

void Model::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Model: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Visible: " << m_visible;
    qDebug().nospace() << tab(l + 1) << "Position: " << m_position;
    qDebug().nospace() << tab(l + 1) << "Rotation: " << m_rotation;
    qDebug().nospace() << tab(l + 1) << "Scaling:  " << m_scaling;
    qDebug("%s%d child mesh(es), %d child model(s)",
           tab(l + 1), m_childMeshes.size(), m_childModels.size());
}

void Model::dumpObjectTree(int l) {
    dumpObjectInfo(l);
    for (int i = 0; i < m_childMeshes.size(); i++)
        m_childMeshes[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_childModels.size(); i++)
        m_childModels[i]->dumpObjectTree(l + 1);
}

bool Model::isGizmo() const {
    return false;
}

bool Model::isLight() const {
    return false;
}

bool Model::isMesh() const {
    return false;
}

bool Model::isModel() const {
    return true;
}

QVector3D Model::centerOfMass() const {
    QVector3D centerOfMass;
    float totalMass = 0;
    for (int i = 0; i < m_childMeshes.size(); i++) {
        centerOfMass += m_childMeshes[i]->centerOfMass() * m_childMeshes[i]->mass();
        totalMass += m_childMeshes[i]->mass();
    }
    for (int i = 0; i < m_childModels.size(); i++) {
        centerOfMass += m_childModels[i]->centerOfMass() * m_childModels[i]->mass();
        totalMass += m_childModels[i]->mass();
    }
    return centerOfMass / totalMass;
}

float Model::mass() const {
    float totalMass = 0;
    for (int i = 0; i < m_childMeshes.size(); i++)
        totalMass += m_childMeshes[i]->mass();
    for (int i = 0; i < m_childModels.size(); i++)
        totalMass += m_childModels[i]->mass();
    return totalMass;
}

Mesh * Model::assemble() const {
    Mesh* assembledMesh = 0;
    for (int i = 0; i < m_childMeshes.size(); i++) {
        Mesh* old = assembledMesh;
        assembledMesh = Mesh::merge(old, m_childMeshes[i]);
        if (old) delete old;
    }
    for (int i = 0; i < m_childModels.size(); i++) {
        Mesh* old1 = assembledMesh;
        Mesh* old2 = m_childModels[i]->assemble();
        assembledMesh = Mesh::merge(old1, old2);
        if (old1) delete old1;
        if (old2) delete old2;
    }
    return assembledMesh;
}

const QVector<Mesh*>& Model::childMeshes() const {
    return m_childMeshes;
}

const QVector<Model*>& Model::childModels() const {
    return m_childModels;
}

void Model::reverseNormals() {
    for (int i = 0; i < m_childMeshes.size(); i++)
        m_childMeshes[i]->reverseNormals();
    for (int i = 0; i < m_childModels.size(); i++)
        m_childModels[i]->reverseNormals();
}

void Model::reverseTangents() {
    for (int i = 0; i < m_childMeshes.size(); i++)
        m_childMeshes[i]->reverseTangents();
    for (int i = 0; i < m_childModels.size(); i++)
        m_childModels[i]->reverseTangents();
}

void Model::reverseBitangents() {
    for (int i = 0; i < m_childMeshes.size(); i++)
        m_childMeshes[i]->reverseBitangents();
    for (int i = 0; i < m_childModels.size(); i++)
        m_childModels[i]->reverseBitangents();
}

void Model::childEvent(QChildEvent * e) {
    if (e->added()) {
        if (Model* model = qobject_cast<Model*>(e->child()))
            addChildModel(model);
        else if (Mesh* mesh = qobject_cast<Mesh*>(e->child()))
            addChildMesh(mesh);
    } else if (e->removed()) {
        if (removeChildModel(e->child(), false)) return;
        if (removeChildMesh(e->child(), false)) return;
    }
}
