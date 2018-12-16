#pragma once

#include <Generic/Camera.h>
#include <Generic/Model.h>
#include <Generic/Light.h>

class Scene: public QObject {
public:
    Scene(QObject * parent = 0);
    ~Scene();

    void clean();
    void addModel(Model* newModel);
    void addLight(Light* newLight);
    
    vector<Model*> getModels();
    vector<Light*> getLights();
    Camera* getCamera();
    
    static Scene* currentScene();

private:
    Camera * camera;
    vector<Model*> models;
    vector<Light*> lights;
};
