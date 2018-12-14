#pragma once

#include <Generic/Mesh.h>

class Model: public Object {
public:
    Model();
    ~Model();

    void addMesh(Mesh* newMesh);
    void addChildren(Model* newModel);

    vector<Mesh*> getMeshes();
    vector<Model*> getChildren();

private:
    vector<Mesh*> meshes;
    vector<Model*> children;

    void initID() override;
};