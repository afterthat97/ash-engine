#pragma once

#include <Model.h>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

class ModelExporter {
public:
    ModelExporter();
    ~ModelExporter();

    void saveToFile(Model* model, QString filePath);
    void saveToFile(Mesh* mesh, QString filePath);

    bool hasLog();
    QString log();

private:
    QString m_log;

    Model* m_model;
    aiScene* m_aiScenePtr;
    QVector<aiMesh*> m_tmp_aiMeshes;
    QVector<aiMaterial*> m_tmp_aiMaterials;
    QVector<QSharedPointer<Texture>> m_tmp_textures;

    void getAllTextures(Model* model);
    aiNode* exportModel(Model* model);
    aiMesh* exportMesh(Mesh* mesh);
    aiMaterial* exportMaterial(Material* material);
};
