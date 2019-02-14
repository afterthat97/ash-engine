#pragma once

#include <TransformGizmo.h>
#include <Camera.h>
#include <Gridline.h>
#include <AmbientLight.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>
#include <Model.h>

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

    TransformGizmo* transformGizmo() const;
    Camera* camera() const;
    const QVector<Gridline*>& gridlines() const;
    const QVector<AmbientLight*>& ambientLights() const;
    const QVector<DirectionalLight*>& directionalLights() const;
    const QVector<PointLight*>& pointLights() const;
    const QVector<SpotLight*>& spotLights() const;
    const QVector<Model*>& models() const;

signals:
    void cameraChanged(Camera* camera);
    void gridlineAdded(Gridline* gridline);
    void gridlineRemoved(QObject* object);
    void lightAdded(AbstractLight* light);
    void lightRemoved(QObject* object);
    void modelAdded(Model* model);
    void modelRemoved(QObject* object);

protected:
    void childEvent(QChildEvent *event) override;

private:
    TransformGizmo * m_gizmo;
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
