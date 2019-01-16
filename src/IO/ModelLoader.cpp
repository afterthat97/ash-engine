#include <IO/ModelLoader.h>
#include <IO/TextureLoader.h>

// Assimp: 3D model loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelLoader::ModelLoader() {
    m_aiScenePtr = 0;
    textureLoader = new TextureLoader;
}

ModelLoader::~ModelLoader() {
    delete textureLoader;
}

Model * ModelLoader::loadFromFile(QString filePath) {
    if (filePath == "") {
        m_log += "Invalid file path.";
        return 0;
    }

    Assimp::Importer importer;
    unsigned int flags =
        aiProcess_Triangulate |
        aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_OptimizeGraph |
        aiProcess_GenUVCoords;

    if (filePath[0] == ':') { // qrc
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QByteArray bytes = file.readAll();
        m_aiScenePtr = importer.ReadFileFromMemory(bytes.constData(), bytes.length(), flags);
    } else {
        filePath.replace('\\', '/');
        for (int i = 0; i < filePath.length(); i++)
            if (filePath[i] == '/')
                m_dir = filePath.mid(0, i);
        m_aiScenePtr = importer.ReadFile(filePath.toStdString(), flags);
    }

    if (!m_aiScenePtr || !m_aiScenePtr->mRootNode || m_aiScenePtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        m_log += importer.GetErrorString();
        return 0;
    }

    //delete m_aiScenePtr;
    //m_aiScenePtr = 0;

    return loadModel(m_aiScenePtr->mRootNode);
}

Model * ModelLoader::loadConeModel() {
    static ModelLoader * loader = new ModelLoader;
    Model* model = loader->loadFromFile(":/resources/shapes/Cone.fbx");
    return model;
}

Model * ModelLoader::loadCubeModel() {
    static ModelLoader * loader = new ModelLoader;
    Model* model = loader->loadFromFile(":/resources/shapes/Cube.fbx");
    return model;
}

Model * ModelLoader::loadCylinderModel() {
    static ModelLoader * loader = new ModelLoader;
    Model* model = loader->loadFromFile(":/resources/shapes/Cylinder.fbx");
    return model;
}

Model * ModelLoader::loadPlaneModel() {
    static ModelLoader * loader = new ModelLoader;
    Model* model = loader->loadFromFile(":/resources/shapes/Plane.fbx");
    return model;
}

Model * ModelLoader::loadSphereModel() {
    static ModelLoader * loader = new ModelLoader;
    Model* model = loader->loadFromFile(":/resources/shapes/Sphere.fbx");
    return model;
}

bool ModelLoader::hasLog() {
    return m_log.length() != 0 || textureLoader->hasLog();
}

QString ModelLoader::log() {
    QString tmp = m_log + textureLoader->log();
    m_log = "";
    return tmp;
}

Model * ModelLoader::loadModel(const aiNode * aiNodePtr) {
    Model* model = new Model;
    model->setObjectName(aiNodePtr->mName.length ? aiNodePtr->mName.C_Str() : "Untitled");
    for (uint32_t i = 0; i < aiNodePtr->mNumMeshes; i++)
        model->addChildMesh(loadMesh(m_aiScenePtr->mMeshes[aiNodePtr->mMeshes[i]]));
    for (uint32_t i = 0; i < aiNodePtr->mNumChildren; i++)
        model->addChildModel(loadModel(aiNodePtr->mChildren[i]));
    return model;
}

Mesh * ModelLoader::loadMesh(const aiMesh * aiMeshPtr) {
    QVector<Vertex> vertices;
    QVector<uint32_t> indices;

    for (uint32_t i = 0; i < aiMeshPtr->mNumVertices; i++) {
        Vertex vertex;
        if (aiMeshPtr->HasPositions())
            vertex.position = QVector3D(aiMeshPtr->mVertices[i].x, aiMeshPtr->mVertices[i].y, aiMeshPtr->mVertices[i].z);
        if (aiMeshPtr->HasNormals())
            vertex.normal = QVector3D(aiMeshPtr->mNormals[i].x, aiMeshPtr->mNormals[i].y, aiMeshPtr->mNormals[i].z);
        if (aiMeshPtr->HasTangentsAndBitangents()) {
            // Assimp use left-handed tangent space
            vertex.tangent = QVector3D(aiMeshPtr->mTangents[i].x, aiMeshPtr->mTangents[i].y, aiMeshPtr->mTangents[i].z);
            vertex.bitangent = QVector3D(aiMeshPtr->mBitangents[i].x, aiMeshPtr->mBitangents[i].y, aiMeshPtr->mBitangents[i].z);
            
            // Gram-Schmidt process, re-orthogonalize the TBN vectors
            vertex.tangent -= QVector3D::dotProduct(vertex.tangent, vertex.normal) * vertex.normal;
            vertex.tangent.normalize();

            // Deal with mirrored texture coordinates
            if (QVector3D::dotProduct(QVector3D::crossProduct(vertex.tangent, vertex.normal), vertex.bitangent) < 0.0f)
                vertex.tangent = -vertex.tangent;
        }
        if (aiMeshPtr->HasTextureCoords(0))
            vertex.texCoords = QVector2D(aiMeshPtr->mTextureCoords[0][i].x, aiMeshPtr->mTextureCoords[0][i].y);
        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < aiMeshPtr->mNumFaces; i++)
        for (uint32_t j = 0; j < 3; j++)
            indices.push_back(aiMeshPtr->mFaces[i].mIndices[j]);

    Mesh* mesh = new Mesh;
    mesh->setObjectName(aiMeshPtr->mName.length ? aiMeshPtr->mName.C_Str() : "Untitled");
    mesh->setGeometry(vertices, indices);
    mesh->setMaterial(loadMaterial(m_aiScenePtr->mMaterials[aiMeshPtr->mMaterialIndex]));

    return mesh;
}

Material * ModelLoader::loadMaterial(const aiMaterial * aiMaterialPtr) {
    Material* material = new Material;
    aiColor4D color; float value; aiString aiStr;

    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_NAME, aiStr))
        material->setObjectName(aiStr.length ? aiStr.C_Str() : "Untitled");
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_AMBIENT, color))
        material->setAmbient((color.r + color.g + color.b) / 3.0f);
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
        material->setDiffuse((color.r + color.g + color.b) / 3.0f);
        material->setColor(QVector3D(color.r, color.g, color.b) / material->diffuse());
    }
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_SPECULAR, color))
        material->setSpecular((color.r + color.g + color.b) / 3.0f);
    if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_SHININESS, value) && !qFuzzyIsNull(value))
        material->setShininess(value);
    if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_DIFFUSE, 0, &aiStr)) {
        QString filePath = m_dir + '/' + QString(aiStr.C_Str()).replace('\\', '/');
        material->setDiffuseTexture(textureLoader->loadFromFile(Texture::Diffuse, filePath));
    }
    if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_SPECULAR, 0, &aiStr)) {
        QString filePath = m_dir + '/' + QString(aiStr.C_Str()).replace('\\', '/');
        material->setSpecularTexture(textureLoader->loadFromFile(Texture::Specular, filePath));
    }
    if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_HEIGHT, 0, &aiStr)) {
        QString filePath = m_dir + '/' + QString(aiStr.C_Str()).replace('\\', '/');
        material->setBumpTexture(textureLoader->loadFromFile(Texture::Bump, filePath));
    }
    return material;
}
