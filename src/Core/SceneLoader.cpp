#include <SceneLoader.h>

SceneLoader::SceneLoader() {}

Scene * SceneLoader::loadFromFile(QString filePath) {
    m_textures.clear();

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);

    if (!file.isOpen()) {
        if (log_level >= LOG_LEVEL_ERROR)
            dout << "Failed to load file:" << file.errorString();
        m_log += file.errorString() + "\n";
        return 0;
    }

    QDataStream in(&file);

    quint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != 0xA0B0C0D0) {
        if (log_level >= LOG_LEVEL_ERROR)
            dout << "Failed to load file: Invalid File Format";
        m_log += "Invalid File Format.\n";
        return 0;
    }

    quint32 versionNumber;
    in >> versionNumber;
    if (versionNumber > 100) {
        if (log_level >= LOG_LEVEL_ERROR)
            dout << "Failed to load file: Version not supported";
        m_log += "Version not supported.\n";
        return 0;
    }

    int textureNum;
    in >> textureNum;
    for (int i = 0; i < textureNum; i++) {
        QSharedPointer<Texture> texture(loadTexture(in));
        m_textures.push_back(texture);
    }

    int cameraNum;
    in >> cameraNum;
    if (cameraNum != 1) {
        if (log_level >= LOG_LEVEL_ERROR)
            dout << "Failed to load file: Unknown error";
        m_log += "Unknown error.\n";
        return 0;
    }

    Scene* scene = new Scene;
    scene->setCamera(loadCamera(in));

    int gridlineNum;
    in >> gridlineNum;
    for (int i = 0; i < gridlineNum; i++) {
        Gridline* gridline = loadGridline(in);
        scene->m_gridlines.push_back(gridline);
        gridline->setParent(scene);
    }

    int ambientLightNum;
    in >> ambientLightNum;
    for (int i = 0; i < ambientLightNum; i++) {
        AmbientLight* light = loadAmbientLight(in);
        scene->m_ambientLights.push_back(light);
        light->setParent(scene);
    }

    int directionalLightNum;
    in >> directionalLightNum;
    for (int i = 0; i < directionalLightNum; i++) {
        DirectionalLight* light = loadDirectionalLight(in);
        scene->m_directionalLights.push_back(light);
        light->setParent(scene);
    }

    int pointLightNum;
    in >> pointLightNum;
    for (int i = 0; i < pointLightNum; i++) {
        PointLight* light = loadPointLight(in);
        scene->m_pointLights.push_back(light);
        light->setParent(scene);
    }

    int spotLightNum;
    in >> spotLightNum;
    for (int i = 0; i < spotLightNum; i++) {
        SpotLight* light = loadSpotLight(in);
        scene->m_spotLights.push_back(light);
        light->setParent(scene);
    }

    int modelNum;
    in >> modelNum;
    for (int i = 0; i < modelNum; i++) {
        Model* model = loadModel(in);
        scene->m_models.push_back(model);
        model->setParent(scene);
    }

    in >> scene->m_gridlineNameCounter;
    in >> scene->m_ambientLightNameCounter;
    in >> scene->m_directionalLightNameCounter;
    in >> scene->m_pointLightNameCounter;
    in >> scene->m_spotLightNameCounter;

    return scene;
}

bool SceneLoader::hasErrorLog() {
    return m_log != "";
}

QString SceneLoader::errorLog() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}

Camera * SceneLoader::loadCamera(QDataStream & in) {
    Camera* camera = new Camera;

    float movingSpeed, fieldOfView, aspectRatio, nearPlane, farPlane;
    QVector3D position, direction;

    in >> movingSpeed >> fieldOfView >> aspectRatio >> nearPlane >> farPlane;
    in >> position >> direction;

    camera->setMovingSpeed(movingSpeed);
    camera->setFieldOfView(fieldOfView);
    camera->setAspectRatio(aspectRatio);
    camera->setNearPlane(nearPlane);
    camera->setFarPlane(farPlane);
    camera->setPosition(position);
    camera->setDirection(direction);

    return camera;
}

Gridline * SceneLoader::loadGridline(QDataStream & in) {
    Gridline* gridline = new Gridline;

    QString name;
    QVector3D xargs, yargs, zargs, color;
    in >> name >> xargs >> yargs >> zargs >> color;

    gridline->setObjectName(name);
    gridline->setXArguments(xargs);
    gridline->setYArguments(yargs);
    gridline->setZArguments(zargs);

    return gridline;
}

AmbientLight * SceneLoader::loadAmbientLight(QDataStream & in) {
    AmbientLight* light = new AmbientLight;

    QString name;
    QVector3D color;
    bool enabled;
    float intensity;
    in >> name >> color >> enabled >> intensity;

    light->setObjectName(name);
    light->setColor(color);
    light->setEnabled(enabled);
    light->setIntensity(intensity);

    return light;
}

DirectionalLight * SceneLoader::loadDirectionalLight(QDataStream & in) {
    DirectionalLight* light = new DirectionalLight;

    QString name;
    QVector3D color, direction;
    bool enabled;
    float intensity;
    in >> name >> color >> enabled >> intensity >> direction;

    light->setObjectName(name);
    light->setColor(color);
    light->setEnabled(enabled);
    light->setIntensity(intensity);
    light->setDirection(direction);

    return light;
}

PointLight * SceneLoader::loadPointLight(QDataStream & in) {
    PointLight* light = new PointLight;

    QString name;
    QVector3D color, position, attenuationArgs;
    bool enabled, enableAttenuation;
    float intensity;

    in >> name >> color >> enabled >> intensity;
    in >> position >> enableAttenuation >> attenuationArgs;

    light->setObjectName(name);
    light->setColor(color);
    light->setEnabled(enabled);
    light->setIntensity(intensity);
    light->setPosition(position);
    light->setEnableAttenuation(enableAttenuation);
    light->setAttenuationArguments(attenuationArgs);

    return light;
}

SpotLight * SceneLoader::loadSpotLight(QDataStream & in) {
    SpotLight* light = new SpotLight;

    QString name;
    QVector3D color, position, direction, attenuationArgs;
    bool enabled, enableAttenuation;
    float intensity, innerCutOff, outerCutOff;

    in >> name >> color >> enabled >> intensity;
    in >> position >> direction >> innerCutOff >> outerCutOff;
    in >> enableAttenuation >> attenuationArgs;

    light->setObjectName(name);
    light->setColor(color);
    light->setEnabled(enabled);
    light->setIntensity(intensity);
    light->setPosition(position);
    light->setDirection(direction);
    light->setInnerCutOff(innerCutOff);
    light->setOuterCutOff(outerCutOff);
    light->setEnableAttenuation(enableAttenuation);
    light->setAttenuationArguments(attenuationArgs);

    return light;
}

Model * SceneLoader::loadModel(QDataStream & in) {
    Model* model = new Model;

    QString name;
    bool visible;
    QVector3D position, rotation, scaling;
    in >> name >> visible >> position >> rotation >> scaling;

    model->setObjectName(name);
    model->setVisible(visible);
    model->setPosition(position);
    model->setRotation(rotation);
    model->setScaling(scaling);

    int childMeshNum;
    in >> childMeshNum;
    for (int i = 0; i < childMeshNum; i++) {
        Mesh* mesh = loadMesh(in);
        model->addChildMesh(mesh);
    }

    int childModelNum;
    in >> childModelNum;
    for (int i = 0; i < childModelNum; i++) {
        Model* model = loadModel(in);
        model->addChildModel(model);
    }

    return model;
}

Mesh * SceneLoader::loadMesh(QDataStream & in) {
    Mesh* mesh = new Mesh;

    QString name;
    bool visible;
    Mesh::MeshType meshType;
    QVector3D position, rotation, scaling;
    QVector<Vertex> vertices;
    QVector<uint32_t> indices;

    in >> name >> visible >> meshType;
    in >> position >> rotation >> scaling;
    in >> vertices >> indices;

    mesh->setObjectName(name);
    mesh->setVisible(visible);
    mesh->setMeshType(meshType);
    mesh->setPosition(position);
    mesh->setRotation(rotation);
    mesh->setScaling(scaling);
    mesh->setGeometry(vertices, indices);

    bool hasMaterial;
    in >> hasMaterial;
    if (hasMaterial) {
        mesh->setMaterial(loadMaterial(in));
    }

    return mesh;
}

Material * SceneLoader::loadMaterial(QDataStream & in) {
    Material* material = new Material;

    QString name;
    QVector3D color;
    float ambient, diffuse, specular, shininess;

    in >> name >> color;
    in >> ambient >> diffuse >> specular >> shininess;

    material->setObjectName(name);
    material->setColor(color);
    material->setAmbient(ambient);
    material->setDiffuse(diffuse);
    material->setSpecular(specular);
    material->setShininess(shininess);

    int textureNum;
    in >> textureNum;
    for (int i = 0; i < textureNum; i++) {
        Texture::TextureType textureType;
        qint32 indx;
        in >> textureType >> indx;
        if (textureType == Texture::Diffuse) {
            material->setDiffuseTexture(m_textures[indx]);
        } else if (textureType == Texture::Specular) {
            material->setSpecularTexture(m_textures[indx]);
        } else {
            material->setBumpTexture(m_textures[indx]);
        }
    }

    return material;
}

QSharedPointer<Texture> SceneLoader::loadTexture(QDataStream & in) {
    Texture* texture = new Texture;

    QString name;
    bool enabled;
    Texture::TextureType textureType;
    QImage image;

    in >> name >> enabled >> textureType >> image;

    texture->setObjectName(name);
    texture->setEnabled(enabled);
    texture->setTextureType(textureType);
    texture->setImage(image);

    return QSharedPointer<Texture>(texture);
}
