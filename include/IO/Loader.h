#pragma once

// Assimp: 3D model loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Generic/Scene.h>

class Loader {
public:
    static Model* loadFromFile(QString filepath);

private:
    static QString dir;
    static vector<QString> errorFiles;
    static const aiScene* aiScenePtr;

    static Model* loadModel(const aiNode* aiNodePtr);
    static Mesh* loadMesh(const aiMesh* aiMeshPtr);
    static Material* loadMaterial(const aiMaterial* aiMaterialPtr);
    static Texture* loadTexture(Texture::TextureType textureType, QString filename);
};
