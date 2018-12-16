#include <Generic/Scene.h>

Scene::Scene(QObject * parent): QObject(parent) {
    setObjectName("Untitled Scene");
    camera = new Camera(QVector3D(0, 2, 10), QVector3D(0, 0, -1), this);
}

Scene::~Scene() {
    delete camera;
    for (uint32_t i = 0; i < models.size(); i++)
        delete models[i];
    for (uint32_t i = 0; i < lights.size(); i++)
        delete lights[i];
}

void Scene::addModel(Model * newModel) {
    if (!newModel) return;
    models.push_back(newModel);
    newModel->setParent(this);
}

void Scene::addLight(Light * newLight) {
    if (!newLight) return;
    lights.push_back(newLight);
    newLight->setParent(this);
}

vector<Model*> Scene::getModels() {
    return models;
}

vector<Light*> Scene::getLights() {
    return lights;
}

Camera * Scene::getCamera() {
    return camera;
}

void Scene::clean() {
    for (uint32_t i = 0; i < models.size(); i++)
        delete models[i];
    for (uint32_t i = 0; i < lights.size(); i++)
        delete lights[i];
    setObjectName("Untitled Scene");
    models.clear();
    lights.clear();
    camera->reset();
}

Scene * Scene::currentScene() {
    static Scene* scene = new Scene;
    return scene;
}
