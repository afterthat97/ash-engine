#include <Core/Scene.h>
#include <Core/Light/AmbientLight.h>
#include <Core/Light/DirectionalLight.h>
#include <Core/Light/PointLight.h>
#include <Core/Light/SpotLight.h>

Scene::Scene(): QObject(0) {
    setObjectName("Untitled Scene");
    m_camera = new Camera(this);
}

// Add & remove members

Scene::Scene(const Scene & scene) {
    m_camera = new Camera(*scene.m_camera);
    for (int i = 0; i < scene.m_gridlines.size(); i++)
        addGridline(new Gridline(*scene.m_gridlines[i]));
    for (int i = 0; i < scene.lights().size(); i++)
        addLight(scene.lights()[i]);
    for (int i = 0; i < scene.m_models.size(); i++)
        addModel(new Model(*scene.m_models[i]));
}

bool Scene::setCamera(Camera * camera) {
    if (camera == 0 || m_camera == camera)
        return false;
    delete m_camera;
    m_camera = camera;
    m_camera->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::addGridline(Gridline * gridline) {
    if (!gridline || m_gridlines.contains(gridline))
        return false;
    m_gridlines.push_back(gridline);
    gridline->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::addLight(AbstractLight * l) {
    if (SpotLight* light = qobject_cast<SpotLight*>(l)) {
        return addSpotLight(light);
    } else if (AmbientLight* light = qobject_cast<AmbientLight*>(l)) {
        return addAmbientLight(light);
    } else if (DirectionalLight* light = qobject_cast<DirectionalLight*>(l)) {
        return addDirectionalLight(light);
    } else if (PointLight* light = qobject_cast<PointLight*>(l)) {
        return addPointLight(light);
    }
    return false;
}

bool Scene::addAmbientLight(AmbientLight * light) {
    if (!light || m_ambientLights.contains(light))
        return false;
    m_ambientLights.push_back(light);
    light->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::addDirectionalLight(DirectionalLight * light) {
    if (!light || m_directionalLights.contains(light))
        return false;
    m_directionalLights.push_back(light);
    light->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::addPointLight(PointLight * light) {
    if (!light || m_pointLights.contains(light))
        return false;
    m_pointLights.push_back(light);
    light->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::addSpotLight(SpotLight * light) {
    if (!light || m_spotLights.contains(light))
        return false;
    m_spotLights.push_back(light);
    light->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::addModel(Model * model) {
    if (!model || m_models.contains(model))
        return false;
    m_models.push_back(model);
    model->setParent(this);
    sceneChanged();
    return true;
}

bool Scene::removeGridline(Gridline * gridline) {
    for (int i = 0; i < m_gridlines.size(); i++)
        if (m_gridlines[i] == gridline) {
            m_gridlines.erase(m_gridlines.begin() + i);
            sceneChanged();
            return true;
        }
    return false;
}

bool Scene::removeLight(AbstractLight * light) {
    for (int i = 0; i < m_ambientLights.size(); i++)
        if (m_ambientLights[i] == light) {
            m_ambientLights.erase(m_ambientLights.begin() + i);
            sceneChanged();
            return true;
        }
    for (int i = 0; i < m_directionalLights.size(); i++)
        if (m_directionalLights[i] == light) {
            m_directionalLights.erase(m_directionalLights.begin() + i);
            sceneChanged();
            return true;
        }
    for (int i = 0; i < m_pointLights.size(); i++)
        if (m_pointLights[i] == light) {
            m_pointLights.erase(m_pointLights.begin() + i);
            sceneChanged();
            return true;
        }
    for (int i = 0; i < m_spotLights.size(); i++)
        if (m_spotLights[i] == light) {
            m_spotLights.erase(m_spotLights.begin() + i);
            sceneChanged();
            return true;
        }
    return false;
}

bool Scene::removeModel(Model * model) {
    for (int i = 0; i < m_models.size(); i++)
        if (m_models[i] == model) {
            m_models.erase(m_models.begin() + i);
            sceneChanged();
            return true;
        }
    return false;
}

// Dump info

void Scene::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Scene: " << objectName();
    qDebug("%s%d camera, %d gridline(s), %d light(s), %d model(s)",
           tab(l), m_camera != 0, m_gridlines.size(), this->lights().size(), m_models.size());
}

void Scene::dumpObjectTree(int l) {
    dumpObjectInfo(l);
    m_camera->dumpObjectTree(l + 1);
    for (int i = 0; i < m_gridlines.size(); i++)
        m_gridlines[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < this->lights().size(); i++)
        this->lights()[i]->dumpObjectTree(l + 1);
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

QVector<AbstractLight*> Scene::lights() const {
    QVector<AbstractLight*> m_lights;
    for (int i = 0; i < m_ambientLights.size(); i++)
        m_lights.push_back(m_ambientLights[i]);
    for (int i = 0; i < m_directionalLights.size(); i++)
        m_lights.push_back(m_directionalLights[i]);
    for (int i = 0; i < m_pointLights.size(); i++)
        m_lights.push_back(m_pointLights[i]);
    for (int i = 0; i < m_spotLights.size(); i++)
        m_lights.push_back(m_spotLights[i]);
    return m_lights;
}

const QVector<AmbientLight*>& Scene::ambientLights() const {
    return m_ambientLights;
}

const QVector<DirectionalLight*>& Scene::directionalLights() const {
    return m_directionalLights;
}

const QVector<PointLight*>& Scene::pointLights() const {
    return m_pointLights;
}

const QVector<SpotLight*>& Scene::spotLights() const {
    return m_spotLights;
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
            sceneChanged();
        } else if (Gridline* gridline = qobject_cast<Gridline*>(e->child()))
            removeGridline(gridline);
        else if (AbstractLight* light = qobject_cast<AbstractLight*>(e->child()))
            removeLight(light);
        else if (Model* model = qobject_cast<Model*>(e->child()))
            removeModel(model);
    }
}
