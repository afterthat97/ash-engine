#pragma once

#include <Core/Axis.h>
#include <Core/Camera.h>
#include <Core/Gridline.h>
#include <Core/Light/AmbientLight.h>
#include <Core/Light/DirectionalLight.h>
#include <Core/Light/PointLight.h>
#include <Core/Light/SpotLight.h>
#include <Core/Model.h>

class SceneLoader;
class SceneSaver;

class Scene: public QObject {
    Q_OBJECT

public:
    Scene();
    Scene(const Scene& scene);
    ~Scene();

    bool setCamera(Camera* camera);
    bool addGridline(Gridline* gridline);
    bool addLight(AbstractLight* light);
    bool addAmbientLight(AmbientLight* light);
    bool addDirectionalLight(DirectionalLight* light);
    bool addPointLight(PointLight* light);
    bool addSpotLight(SpotLight* light);
    bool addModel(Model* model);

    bool removeGridline(QObject* gridline);
    bool removeLight(QObject* light);
    bool removeModel(QObject* model, bool recursive);

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    Axis* axis() const;
    Camera* camera() const;
    const QVector<Gridline*>& gridlines() const;
    QVector<AbstractLight*> lights() const;
    const QVector<AmbientLight*>& ambientLights() const;
    const QVector<DirectionalLight*>& directionalLights() const;
    const QVector<PointLight*>& pointLights() const;
    const QVector<SpotLight*>& spotLights() const;
    const QVector<Model*>& models() const;

signals:
    void gridlineAdded(Gridline* gridline);
    void gridlineRemoved(QObject* object);
    void lightAdded(AbstractLight* light);
    void lightRemoved(QObject* object);
    void modelAdded(Model* model);
    void modelRemoved(QObject* object);

protected:
    void childEvent(QChildEvent *event) override;

private:
    Axis * m_axis;
    Camera * m_camera;
    QVector<Gridline*> m_gridlines;
    QVector<AmbientLight*> m_ambientLights;
    QVector<DirectionalLight*> m_directionalLights;
    QVector<PointLight*> m_pointLights;
    QVector<SpotLight*> m_spotLights;
    QVector<Model*> m_models;

    int m_gridlineNameCounter;
    int m_ambientLightNameCounter;
    int m_directionalLightNameCounter;
    int m_pointLightNameCounter;
    int m_spotLightNameCounter;

    friend SceneLoader;
    friend SceneSaver;
};
