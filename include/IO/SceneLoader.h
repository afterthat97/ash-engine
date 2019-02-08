#pragma once

#include <Core/Scene.h>

class SceneLoader {
public:
    SceneLoader();

    Scene* loadFromFile(QString filePath);

    bool hasLog();
    QString log();

private:
    Camera* loadCamera(QDataStream& in);
    Gridline* loadGridline(QDataStream& in);
    AmbientLight* loadAmbientLight(QDataStream& in);
    DirectionalLight* loadDirectionalLight(QDataStream& in);
    PointLight* loadPointLight(QDataStream& in);
    SpotLight* loadSpotLight(QDataStream& in);
    Model* loadModel(QDataStream& in);
    Mesh* loadMesh(QDataStream& in);
    Material* loadMaterial(QDataStream& in);
    QSharedPointer<Texture> loadTexture(QDataStream& in);

    QVector<QSharedPointer<Texture>> m_textures;
    QString m_log;
};