#include <Core/Scene.h>

Scene::Scene(): QObject(0) {
    setObjectName("Untitled Scene");
    m_camera = new Camera(this);
}

// Add & remove members

bool Scene::setCamera(Camera * camera) {
    if (camera == 0 || m_camera == camera)
        return false;
    delete m_camera;
    m_camera = camera;
    m_camera->setParent(this);
    cameraChanged(m_camera);
    return true;
}

bool Scene::addGridline(Gridline * gridline) {
    if (!gridline || m_gridlines.contains(gridline))
        return false;
    m_gridlines.push_back(gridline);
    gridline->setParent(this);
    gridlineAdded(gridline);
    return true;
}

bool Scene::addLight(AbstractLight * light) {
    if (!light || m_lights.contains(light))
        return false;
    m_lights.push_back(light);
    light->setParent(this);
    lightAdded(light);
    return true;
}

bool Scene::addModel(Model * model) {
    if (!model || m_models.contains(model))
        return false;
    m_models.push_back(model);
    model->setParent(this);
    modelAdded(model);
    return true;
}

bool Scene::removeGridline(Gridline * gridline) {
    for (int i = 0; i < m_gridlines.size(); i++)
        if (m_gridlines[i] == gridline) {
            m_gridlines.erase(m_gridlines.begin() + i);
            gridlineRemoved(gridline);
            return true;
        }
    return false;
}

bool Scene::removeLight(AbstractLight * light) {
    for (int i = 0; i < m_lights.size(); i++)
        if (m_lights[i] == light) {
            m_lights.erase(m_lights.begin() + i);
            lightRemoved(light);
            return true;
        }
    return false;
}

bool Scene::removeModel(Model * model) {
    for (int i = 0; i < m_models.size(); i++)
        if (m_models[i] == model) {
            m_models.erase(m_models.begin() + i);
            modelAdded(model);
            return true;
        }
    return false;
}

// Dump info

void Scene::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Scene: " << objectName();
    qDebug("%s%d camera, %d gridline(s), %d light(s), %d model(s)",
           tab(l), m_camera != 0, m_gridlines.size(), m_lights.size(), m_models.size());
}

void Scene::dumpObjectTree(int l) {
    dumpObjectInfo(l);
    m_camera->dumpObjectTree(l + 1);
    for (int i = 0; i < m_gridlines.size(); i++)
        m_gridlines[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_lights.size(); i++)
        m_lights[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_models.size(); i++)
        m_models[i]->dumpObjectTree(l + 1);
}

// Get properties

Camera * Scene::camera() const {
    return m_camera;
}

const QVector<Gridline*>& Scene::gridlines() const {
    return m_gridlines;
}

const QVector<AbstractLight*>& Scene::lights() const {
    return m_lights;
}

const QVector<Model*>& Scene::models() const {
    return m_models;
}

// Protected

void Scene::childEvent(QChildEvent * e) {
    if (e->added()) {
        if (Camera* camera = qobject_cast<Camera*>(e->child()))
            setCamera(camera);
        else if (Gridline* gridline = qobject_cast<Gridline*>(e->child()))
            addGridline(gridline);
        else if (AbstractLight* light = qobject_cast<AbstractLight*>(e->child()))
            addLight(light);
        else if (Model* model = qobject_cast<Model*>(e->child()))
            addModel(model);
    } else if (e->removed()) {
        if (Camera* camera = qobject_cast<Camera*>(e->child())) {
            qWarning() << "WARNING: Don't delete the camera returned by scene!";
            m_camera = new Camera(this);
            cameraChanged(m_camera);
        } else if (Gridline* gridline = qobject_cast<Gridline*>(e->child()))
            removeGridline(gridline);
        else if (AbstractLight* light = qobject_cast<AbstractLight*>(e->child()))
            removeLight(light);
        else if (Model* model = qobject_cast<Model*>(e->child()))
            removeModel(model);
    }
}
