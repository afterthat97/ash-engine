#include <IO/Loader.h>
#include <UI/Common.h>

QString Loader::dir = "";

Model * Loader::loadFromFile(QString filepath) {
    if (filepath == "") return NULL;

    const aiScene* aiScenePtr = NULL;
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate |
                         aiProcess_CalcTangentSpace |
                         aiProcess_GenSmoothNormals |
                         aiProcess_JoinIdenticalVertices |
                         aiProcess_OptimizeGraph |
                         aiProcess_GenUVCoords |
                         aiProcess_FlipUVs;

    if (filepath[0] == ':') { // qrc
        QFile file(filepath);
        file.open(QIODevice::ReadOnly);
        QByteArray bytes = file.readAll();
        aiScenePtr = importer.ReadFileFromMemory(bytes.constData(), bytes.length(), flags);
    } else {
        filepath.replace('\\', '/');
        for (int i = 0; i < filepath.length(); i++)
            if (filepath[i] == '/')
                dir = filepath.mid(0, i);
        aiScenePtr = importer.ReadFile(filepath.toStdString(), flags);
    }

    if (!aiScenePtr || !aiScenePtr->mRootNode || aiScenePtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        QMessageBox::critical(NULL, "Error", importer.GetErrorString());
        return NULL;
    }
    return loadModel(aiScenePtr, aiScenePtr->mRootNode);
}

Model * Loader::loadModel(const aiScene* aiScenePtr, const aiNode * aiNodePtr) {
    Model* newModel = new Model();
    newModel->setName(aiNodePtr->mName.C_Str());
    for (uint32_t i = 0; i < aiNodePtr->mNumMeshes; i++)
        newModel->addMesh(loadMesh(aiScenePtr, aiScenePtr->mMeshes[aiNodePtr->mMeshes[i]]));
    for (uint32_t i = 0; i < aiNodePtr->mNumChildren; i++)
        newModel->addChildren(loadModel(aiScenePtr, aiNodePtr->mChildren[i]));
    return newModel;
}

Mesh * Loader::loadMesh(const aiScene* aiScenePtr, const aiMesh * aiMeshPtr) {
    vector<Vertex> vertices;
    for (uint32_t i = 0; i < aiMeshPtr->mNumVertices; i++) {
        Vertex vertex;
        if (aiMeshPtr->HasPositions())
            vertex.position = QVector3D(aiMeshPtr->mVertices[i].x, aiMeshPtr->mVertices[i].y, aiMeshPtr->mVertices[i].z);
        if (aiMeshPtr->HasNormals())
            vertex.normal = QVector3D(aiMeshPtr->mNormals[i].x, aiMeshPtr->mNormals[i].y, aiMeshPtr->mNormals[i].z);
        if (aiMeshPtr->HasTangentsAndBitangents())
            vertex.tangent = QVector3D(aiMeshPtr->mTangents[i].x, aiMeshPtr->mTangents[i].y, aiMeshPtr->mTangents[i].z);
        if (aiMeshPtr->HasTextureCoords(0))
            vertex.texCoords = QVector2D(aiMeshPtr->mTextureCoords[0][i].x, aiMeshPtr->mTextureCoords[0][i].y);
        vertices.push_back(vertex);
    }

    vector<uint32_t> indices;
    for (uint32_t i = 0; i < aiMeshPtr->mNumFaces; i++)
        for (uint32_t j = 0; j < 3; j++)
            indices.push_back(aiMeshPtr->mFaces[i].mIndices[j]);

    Mesh* newMesh = new Mesh;
    newMesh->setName(aiMeshPtr->mName.C_Str());
    newMesh->setVertices(vertices);
    newMesh->setIndices(indices);
    newMesh->setMaterial(loadMaterial(aiScenePtr->mMaterials[aiMeshPtr->mMaterialIndex]));

    return newMesh;
}

Material * Loader::loadMaterial(const aiMaterial * aiMaterialPtr) {
    Material* newMaterial = new Material;
    aiColor4D color; float value; aiString aiStr;

    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_NAME, aiStr))
        newMaterial->setName(aiStr.C_Str());
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_AMBIENT, color))
        newMaterial->setAmbientColor(QVector3D(color.r, color.g, color.b));
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_DIFFUSE, color))
        newMaterial->setDiffuseColor(QVector3D(color.r, color.g, color.b));
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_SPECULAR, color))
        newMaterial->setSpecularColor(QVector3D(color.r, color.g, color.b));
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_SHININESS, value) && value > 0.01f)
        newMaterial->setShininess(value);
    if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_DIFFUSE, 0, &aiStr))
        newMaterial->addTexture(loadTexture(Texture::Diffuse, aiStr.C_Str()));
    if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_SPECULAR, 0, &aiStr))
        newMaterial->addTexture(loadTexture(Texture::Specular, aiStr.C_Str()));
    if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_HEIGHT, 0, &aiStr))
        newMaterial->addTexture(loadTexture(Texture::Normal, aiStr.C_Str()));
    return newMaterial;
}

Texture * Loader::loadTexture(Texture::TextureType textureType, QString filename) {
    Texture* newTexture = new Texture(textureType);

    filename.replace('\\', '/');
    if (filename[0] == '/') filename.remove(0, 1);
    QString filepath = dir + '/' + filename;

    QImage* image = new QImage(filepath);
    if (image->isNull()) {
        QMessageBox::warning(NULL, "Warning", "Failed to load texture: " + filepath);
        return NULL;
    }

    newTexture->setName(filepath);
    newTexture->setImage(image);

    return newTexture;
}
