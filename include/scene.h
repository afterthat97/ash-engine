#ifndef SCENE_H
#define SCENE_H

#include "utilities.h"
#include "shader.h"
#include "model.h"
#include "material.h"
#include "texture.h"

class Scene {
private:
    vec3 pos;
    quat rot;
    mat4 model;
    vector<Model*> models;
    vector<shared_ptr<Material>> materials;
    Mesh* loadMesh(
        const aiMesh* aiMeshPtr,
        btDiscreteDynamicsWorld* dynamicsWorld);
    Model* loadModel(
        const aiNode* aiNodePtr,
        const aiScene* aiScenePtr,
        btDiscreteDynamicsWorld* dynamicsWorld);
    shared_ptr<Material> loadMaterial(
        const aiMaterial* aiMaterialPtr,
        string dir);
public:
    string name;
    Scene(string, btDiscreteDynamicsWorld*);
	~Scene();
    void addModel(Model*);
    void addMaterial(shared_ptr<Material>);
    void show();
    void hide();
    void select();
    void deselect();
    void render(Shader&);
    void recycle();
    void dumpinfo(string);
    void addTranslation(vec3);
    void addRotation(vec3);
    void setPosition(vec3);
    vec3 getPosition();
};

#endif
