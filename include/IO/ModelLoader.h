#pragma once

#include <Core/Core.h>
#include <IO/TextureLoader.h>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();
    
    Model* loadFromFile(QString filePath);

    static Model* loadConeModel();
    static Model* loadCubeModel();
    static Model* loadCylinderModel();
    static Model* loadPlaneModel();
    static Model* loadSphereModel();

    bool hasLog();
    QString log();

private:
    QString m_dir, m_log;
    const aiScene* m_aiScenePtr;
    TextureLoader* textureLoader;

    Model* loadModel(const aiNode* aiNodePtr);
    Mesh* loadMesh(const aiMesh* aiMeshPtr);
    Material* loadMaterial(const aiMaterial* aiMaterialPtr);
};
