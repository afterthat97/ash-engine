#pragma once

#include <UI/Common.h>
#include <UI/CentralWidget/PropertyWidget/StringEditWidget.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>
#include <Generic/Scene.h>

class CameraPropertyWidget: public QWidget {
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
