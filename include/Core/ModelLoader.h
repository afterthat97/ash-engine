#pragma once

#include <TextureLoader.h>
#include <Model.h>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

class ModelLoader {
public:
    ModelLoader();

    Model* loadModelFromFile(QString filePath);
    Mesh* loadMeshFromFile(QString filePath);

    static Model* loadConeModel();
    static Model* loadCubeModel();
    static Model* loadCylinderModel();
    static Model* loadPlaneModel();
    static Model* loadSphereModel();

    bool hasErrorLog();
    QString errorLog();

private:
    QDir m_dir;
    QString m_log;
    TextureLoader textureLoader;

    const aiScene* m_aiScenePtr;

    Model* loadModel(const aiNode* aiNodePtr);
    Mesh* loadMesh(const aiMesh* aiMeshPtr);
    Material* loadMaterial(const aiMaterial* aiMaterialPtr);
};
