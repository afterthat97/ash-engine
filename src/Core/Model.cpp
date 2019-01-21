#include <Core/Model.h>
#include <Core/Mesh.h>

Model::Model(QObject * parent): QObject(0) {
    m_visible = true;
    resetTransformation();
    setParent(parent);
}

// Add & remove members

Model::Model(const Model & model): QObject(0) {
    m_visible = model.m_visible;
    m_position = model.m_position;
    m_rotation = model.m_rotation;
    m_scaling = model.m_scaling;
    for (int i = 0; i < model.m_childMeshes.size(); i++)
        addChildMesh(new Mesh(*model.m_childMeshes[i]));
    for (int i = 0; i < model.m_childModels.size(); i++)
        addChildModel(new Model(*model.m_childModels[i]));
}

bool Model::addChildMesh(Mesh * mesh) {
    if (!mesh || m_childMeshes.contains(mesh))
        return false;
    m_childMeshes.push_back(mesh);
    mesh->setParent(this);
    connect(mesh, SIGNAL(materialChanged(Material*)), this, SIGNAL(childrenChanged()));
    childrenChanged();
    return true;
}

bool Model::addChildModel(Model * model) {
    if (!model || m_childModels.contains(model))
        return false;
    m_childModels.push_back(model);
    model->setParent(this);
    connect(model, SIGNAL(childrenChanged()), this, SIGNAL(childrenChanged()));
    childrenChanged();
    return true;
}

bool Model::removeChildMesh(QObject * mesh, bool recursive) {
    for (int i = 0; i < m_childMeshes.size(); i++)
        if (m_childMeshes[i] == mesh) {
            m_childMeshes.erase(m_childMeshes.begin() + i);
            childrenChanged();
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
            childrenChanged();
            return true;
        }
    if (!recursive) return false;
    for (int i = 0; i < m_childModels.size(); i++)
        if (m_childModels[i]->removeChildModel(model, recursive))
            return true;
    return false;
}

// Transform functions

void Model::translate(QVector3D delta) {
    setPosition(m_position + delta);
}

void Model::rotate(QQuaternion _rotation) {
    setRotation(_rotation * m_rotation);
}

void Model::rotate(QVector3D _rotation) {
    setRotation(QQuaternion::fromEulerAngles(_rotation) * m_rotation);
}

void Model::scale(QVector3D _scaling) {
    setScaling(m_scaling * _scaling);
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

// Others

QVector3D Model::centerOfMass() const {
    QVector3D centerOfMass;
    for (int i = 0; i < m_childMeshes.size(); i++)
        centerOfMass += m_childMeshes[i]->centerOfMass() * m_childMeshes[i]->mass();
    for (int i = 0; i < m_childModels.size(); i++)
        centerOfMass += m_childModels[i]->centerOfMass() * m_childModels[i]->mass();
    return centerOfMass / mass();
}

float Model::mass() const {
    float totalMass = 0;
    for (int i = 0; i < m_childMeshes.size(); i++)
        totalMass += m_childMeshes[i]->mass();
    for (int i = 0; i < m_childModels.size(); i++)
        totalMass += m_childModels[i]->mass();
    return totalMass;
}

// Get properties

bool Model::visible() const {
    return m_visible;
}

QVector3D Model::localPosition() const {
    return m_position;
}

QVector3D Model::globalPosition() const {
    if (Model* par = qobject_cast<Model*>(parent()))
        return par->globalModelMatrix() * localPosition();
    else
        return localPosition();
}

QVector3D Model::localRotation() const {
    return m_rotation;
}

QVector3D Model::localScaling() const {
    return m_scaling;
}

QMatrix4x4 Model::localModelMatrix() const {
    QMatrix4x4 model;
    model.translate(m_position);
    model.rotate(QQuaternion::fromEulerAngles(m_rotation));
    model.scale(m_scaling);
    return model;
}

QMatrix4x4 Model::globalModelMatrix() const {
    if (Model* par = qobject_cast<Model*>(parent()))
        return par->globalModelMatrix() * localModelMatrix();
    else
        return localModelMatrix();
}

const QVector<Mesh*>& Model::childMeshes() const {
    return m_childMeshes;
}

const QVector<Model*>& Model::childModels() const {
    return m_childModels;
}

// Public slots

void Model::resetTransformation() {
    setPosition(QVector3D());
    setRotation(QVector3D());
    setScaling(QVector3D(1.0f, 1.0f, 1.0f));
}

void Model::resetChildrenTransformation() {
    for (int i = 0; i < m_childMeshes.size(); i++)
        m_childMeshes[i]->resetTransformation();
    for (int i = 0; i < m_childModels.size(); i++) {
        m_childModels[i]->resetTransformation();
        m_childModels[i]->resetChildrenTransformation();
    }
}

void Model::setVisible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        visibleChanged(m_visible);
    }
}

void Model::setPosition(QVector3D position) {
    if (!qFuzzyCompare(m_position, position)) {
        m_position = position;
        positionChanged(m_position);
    }
}

void Model::setRotation(QQuaternion rotation) {
    if (!qFuzzyCompare(m_rotation, rotation.toEulerAngles())) {
        m_rotation = rotation.toEulerAngles();
        rotationChanged(m_rotation);
    }
}

void Model::setRotation(QVector3D rotation) {
    if (!qFuzzyCompare(m_rotation, rotation)) {
        m_rotation = rotation;
        rotationChanged(m_rotation);
    }
}

void Model::setScaling(QVector3D scaling) {
    if (!qFuzzyCompare(m_scaling, scaling)) {
        m_scaling = scaling;
        scalingChanged(m_scaling);
    }
}

// Protected

void Model::childEvent(QChildEvent * e) {
    if (e->added()) {
        if (Model* model = qobject_cast<Model*>(e->child()))
            addChildModel(model);
        else if (Mesh* mesh = qobject_cast<Mesh*>(e->child()))
            addChildMesh(mesh);
    } else if (e->removed()) {
        removeChildModel(e->child(), false);
        removeChildMesh(e->child(), false);
    }
}
