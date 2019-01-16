#pragma once

#include <Core/Camera.h>
#include <Core/Light/AbstractLight.h>
#include <Core/Gridline.h>
#include <Core/Model.h>

class Scene: public QObject {
    Q_OBJECT

public:
    Scene();
    // TODO: Copy constructor

    bool setCamera(Camera* camera);
    bool addGridline(Gridline* gridline);
    bool addLight(AbstractLight* light);
    bool addModel(Model* model);

    bool removeGridline(Gridline* gridline);
    bool removeLight(AbstractLight* light);
    bool removeModel(Model* model);

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    Camera* camera() const;
    const QVector<Gridline*>& gridlines() const;
    const QVector<AbstractLight*>& lights() const;
    const QVector<Model*>& models() const;

signals:
    void cameraChanged(Camera* camera);
    void gridlineAdded(Gridline* gridline);
    void gridlineRemoved(Gridline* gridline);
    void lightAdded(AbstractLight* light);
    void lightRemoved(AbstractLight* light);
    void modelAdded(Model* model);
    void modelRemoved(Model* model);

protected:
    void childEvent(QChildEvent *event) override;

private:
    Camera * m_camera;
    QVector<Gridline*> m_gridlines;
    QVector<AbstractLight*> m_lights;
    QVector<Model*> m_models;
};
