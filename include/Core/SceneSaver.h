#pragma once

#include <Scene.h>

class SceneSaver {
public:
    SceneSaver(Scene* scene);

    bool saveToFile(QString filePath);

    bool hasErrorLog();
    QString errorLog();

private:
    void getAllTextures(Model* model);

    void saveCamera(Camera* camera, QDataStream& out);
    void saveGridline(Gridline* gridline, QDataStream& out);
    void saveAmbientLight(AmbientLight* light, QDataStream& out);
    void saveDirectionalLight(DirectionalLight* light, QDataStream& out);
    void savePointLight(PointLight* light, QDataStream& out);
    void saveSpotLight(SpotLight* light, QDataStream& out);
    void saveModel(Model* model, QDataStream& out);
    void saveMesh(Mesh* mesh, QDataStream& out);
    void saveMaterial(Material* material, QDataStream& out);
    void saveTexture(QSharedPointer<Texture> texture, QDataStream& out);

    Scene* m_scene;
    QVector<QSharedPointer<Texture>> m_textures;
    QString m_log;
};
