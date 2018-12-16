#pragma once

#include <UI/Common.h>
#include <UI/CentralWidget/PropertyWidget/StringEditWidget.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>
#include <Generic/Scene.h>

class CameraPropertyWidget;

class ScenePropertyWidget: public QWidget {
    Q_OBJECT

public:
    ScenePropertyWidget(Scene *scene, QWidget *parent = Q_NULLPTR);
    ~ScenePropertyWidget();

private:
    Scene* scene;
    StringEditWidget * nameEditWidget;
    CameraPropertyWidget * cameraPropertyWidget;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void setSceneName(QString name);
};


class CameraPropertyWidget: public QGroupBox {
    Q_OBJECT

public:
    CameraPropertyWidget(Camera* camera, QWidget *parent = Q_NULLPTR);
    ~CameraPropertyWidget();

private:
    Camera * camera;
    FloatEditWidget * movingSpeedEditWidget;
    Vector3DEditWidget * positionEditWidget, *directionEditWidget;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();
};