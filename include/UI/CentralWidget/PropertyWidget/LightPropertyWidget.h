#pragma once

#include <Generic/Light.h>
#include <UI/CentralWidget/PropertyWidget/FloatEditWidget.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>

class LightPropertyWidget: public QWidget {
    Q_OBJECT

public:
    LightPropertyWidget(Light* light, QWidget *parent = Q_NULLPTR);
    ~LightPropertyWidget();

private:
    Light* light;
    Vector3DEditWidget * positionEditWidget, * colorEditWidget, * attenuationEditWidget;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();
};
