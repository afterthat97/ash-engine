#pragma once

#include <Generic/Light.h>
#include <UI/CentralWidget/PropertyWidget/FloatEditWidget.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>

class LightPropertyWidget: public QWidget {
    Q_OBJECT

public:
    LightPropertyWidget(Light* light, QWidget *parent = 0);
    ~LightPropertyWidget();

private:
    Light* light;
    Vector3DEditWidget * positionEditWidget, * colorEditWidget, * attenuationEditWidget;
    QCheckBox * enableAttenuationCheckBox;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void setEnableAttenuation(int state);
};
