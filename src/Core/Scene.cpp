#include <Scene.h>

Scene::Scene(): QObject(0), m_gizmo(0), m_camera(0) {
    setObjectName("Untitled Scene");
    m_gizmo = new TransformGizmo(this);
    m_camera = new Camera(this);
    m_gridlineNameCounter = 1;
    m_ambientLightNameCounter = 1;
    m_directionalLightNameCounter = 1;
    m_pointLightNameCounter = 1;
    m_spotLightNameCounter = 1;
}

// Add & remove members

Scene::Scene(const Scene & scene): QObject(0) {
    setObjectName(scene.objectName());

    m_gizmo = new TransformGizmo(this);
    m_camera = new Camera(*scene.m_camera);
    m_gridlineNameCounter = scene.m_gridlineNameCounter;
    m_ambientLightNameCounter = scene.m_ambientLightNameCounter;
    m_directionalLightNameCounter = scene.m_directionalLightNameCounter;
    m_pointLightNameCounter = scene.m_pointLightNameCounter;
    m_spotLightNameCounter = scene.m_spotLightNameCounter;

    for (int i = 0; i < scene.m_gridlines.size(); i++)
        addGridline(new Gridline(*scene.m_gridlines[i]));
    for (int i = 0; i < scene.m_ambientLights.size(); i++)
        addAmbientLight(new AmbientLight(*scene.m_ambientLights[i]));
    for (int i = 0; i < scene.m_directionalLights.size(); i++)
        addDirectionalLight(new DirectionalLight(*scene.m_directionalLights[i]));
    for (int i = 0; i < scene.m_pointLights.size(); i++)
        addPointLight(new PointLight(*scene.m_pointLights[i]));
    for (int i = 0; i < scene.m_spotLights.size(); i++)
        addSpotLight(new SpotLight(*scene.m_spotLights[i]));
    for (int i = 0; i < scene.m_models.size(); i++)
        addModel(new Model(*scene.m_models[i]));
}

Scene::~Scene() {
#ifdef DEBUG_OUTPUT
    dout << "Scene" << objectName() << "is destroyed";
#endif
}

bool Scene::setCamera(Camera * camera) {
    if (m_camera == camera) return false;

    if (m_camera) {
        Camera* tmp = m_camera;
        m_camera = 0;
        delete tmp;
    }

    if (camera) {
        m_camera = camera;
        m_camera->setParent(this);
#ifdef DEBUG_OUTPUT
        dout << "Camera" << camera->objectName() << "is assigned to scene" << objectName();
#endif
    }

    cameraChanged(m_camera);
    return true;
}

bool Scene::addGridline(Gridline * gridline) {
    if (!gridline || m_gridlines.contains(gridline))
        return false;

    m_gridlines.push_back(gridline);
    gridline->setParent(this);
    gridline->setObjectName("Gridline" + QString::number(m_gridlineNameCounter++));
    gridlineAdded(gridline);

#ifdef DEBUG_OUTPUT
    dout << "Gridline" << gridline->objectName() << "is added to scene" << objectName();
#endif

    return true;
}

bool Scene::addLight(AbstractLight * l) {
    if (SpotLight* light = qobject_cast<SpotLight*>(l))
        return addSpotLight(light);
    else if (AmbientLight* light = qobject_cast<AmbientLight*>(l))
        return addAmbientLight(light);
    else if (DirectionalLight* light = qobject_cast<DirectionalLight*>(l))
        return addDirectionalLight(light);
    else if (PointLight* light = qobject_cast<PointLight*>(l))
        return addPointLight(light);
    return false;
}

bool Scene::addAmbientLight(AmbientLight * light) {
    if (!light || m_ambientLights.contains(light))
        return false;

    m_ambientLights.push_back(light);
    light->setParent(this);
    light->setObjectName("AmbientLight" + QString::number(m_ambientLightNameCounter++));
    lightAdded(light);

#ifdef DEBUG_OUTPUT
    dout << "Ambient light" << light->objectName() << "is added to scene" << objectName();
#endif

    return true;
}

bool Scene::addDirectionalLight(DirectionalLight * light) {
    if (!light || m_directionalLights.contains(light))
        return false;

    m_directionalLights.push_back(light);
    light->setParent(this);
    light->setObjectName("DirectionalLight" + QString::number(m_directionalLightNameCounter++));
    lightAdded(light);

#ifdef DEBUG_OUTPUT
    dout << "Directional light" << light->objectName() << "is added to scene" << objectName();
#endif
    
    return true;
}

bool Scene::addPointLight(PointLight * light) {
    if (!light || m_pointLights.contains(light))
        return false;

    m_pointLights.push_back(light);
    light->setParent(this);
    light->setObjectName("PointLight" + QString::number(m_pointLightNameCounter++));
    lightAdded(light);

#ifdef DEBUG_OUTPUT
    dout << "Point light" << light->objectName() << "is added to scene" << objectName();
#endif

    return true;
}

bool Scene::addSpotLight(SpotLight * light) {
    if (!light || m_spotLights.contains(light))
        return false;

    m_spotLights.push_back(light);
    light->setParent(this);
    light->setObjectName("SpotLight" + QString::number(m_spotLightNameCounter++));
    lightAdded(light);

#ifdef DEBUG_OUTPUT
    dout << "Spot light" << light->objectName() << "is added to scene" << objectName();
#endif

    return true;
}

bool Scene::addModel(Model * model) {
    if (!model || m_models.contains(model))
        return false;

    m_models.push_back(model);
    model->setParent(this);
    modelAdded(model);

#ifdef DEBUG_OUTPUT
    dout << "Model" << model->objectName() << "is added to scene" << objectName();
#endif

    return true;
}

bool Scene::removeGridline(QObject * gridline) {
    for (int i = 0; i < m_gridlines.size(); i++)
        if (m_gridlines[i] == gridline) {
            m_gridlines.erase(m_gridlines.begin() + i);
            gridlineRemoved(gridline);
#ifdef DEBUG_OUTPUT
            dout << "Gridline" << gridline->objectName() << "is removed from scene" << objectName();
#endif
            return true;
        }
    return false;
}

bool Scene::removeLight(QObject * light) {
    for (int i = 0; i < m_ambientLights.size(); i++)
        if (m_ambientLights[i] == light) {
            m_ambientLights.erase(m_ambientLights.begin() + i);
            lightRemoved(light);
#ifdef DEBUG_OUTPUT
            dout << "Ambient light" << light->objectName() << "is removed from scene" << objectName();
#endif
            return true;
        }
    for (int i = 0; i < m_directionalLights.size(); i++)
        if (m_directionalLights[i] == light) {
            m_directionalLights.erase(m_directionalLights.begin() + i);
            lightRemoved(light);
#ifdef DEBUG_OUTPUT
            dout << "Directional light" << light->objectName() << "is removed from scene" << objectName();
#endif
            return true;
        }
    for (int i = 0; i < m_pointLights.size(); i++)
        if (m_pointLights[i] == light) {
            m_pointLights.erase(m_pointLights.begin() + i);
            lightRemoved(light);
#ifdef DEBUG_OUTPUT
            dout << "Point light" << light->objectName() << "is removed from scene" << objectName();
#endif
            return true;
        }
    for (int i = 0; i < m_spotLights.size(); i++)
        if (m_spotLights[i] == light) {
            m_spotLights.erase(m_spotLights.begin() + i);
            lightRemoved(light);
#ifdef DEBUG_OUTPUT
            dout << "Spot light" << light->objectName() << "is removed from scene" << objectName();
#endif
            return true;
        }
    return false;
}

bool Scene::removeModel(QObject * model, bool recursive) {
    for (int i = 0; i < m_models.size(); i++)
        if (m_models[i] == model) {
            m_models.erase(m_models.begin() + i);
            modelRemoved(model);
#ifdef DEBUG_OUTPUT
            dout << "Model" << model->objectName() << "is removed from scene" << objectName();
#endif
            return true;
        }
    if (!recursive) return false;
    for (int i = 0; i < m_models.size(); i++)
        if (m_models[i]->removeChildModel(model, recursive))
            return true;
    return false;
}

// Dump info

void Scene::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Scene: " << objectName();
    qDebug("%s%d gridline(s), %d ambient light(s), %d directional light(s), %d point light(s), %d spot lights(s), %d model(s)",
           tab(l),
           m_gridlines.size(),
           m_ambientLights.size(),
           m_directionalLights.size(),
           m_pointLights.size(),
           m_spotLights.size(),
           m_models.size());
}

void Scene::dumpObjectTree(int l) {
    dumpObjectInfo(l);
    m_gizmo->dumpObjectTree(l + 1);
    m_camera->dumpObjectTree(l + 1);
    for (int i = 0; i < m_gridlines.size(); i++)
        m_gridlines[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_ambientLights.size(); i++)
        m_ambientLights[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_directionalLights.size(); i++)
        m_directionalLights[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_pointLights.size(); i++)
        m_pointLights[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_spotLights.size(); i++)
        m_spotLights[i]->dumpObjectTree(l + 1);
    for (int i = 0; i < m_models.size(); i++)
        m_models[i]->dumpObjectTree(l + 1);
}

// Get properties

TransformGizmo * Scene::transformGizmo() const {
    return m_gizmo;
}

Camera * Scene::camera() const {
    return m_camera;
}

const QVector<Gridline*>& Scene::gridlines() const {
    return m_gridlines;
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
        if (m_camera == e->child()) {
            m_camera = 0;
            cameraChanged(0);
            return;
        }
        if (removeGridline(e->child())) return;
        if (removeLight(e->child())) return;
        if (removeModel(e->child(), false)) return;
    }
}
