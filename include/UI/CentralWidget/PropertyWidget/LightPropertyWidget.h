#pragma once

#include <Generic/Light.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DWidget.h>

class AttenuationWidget;

class LightPropertyWidget: public QWidget {
    Q_OBJECT

public:
    LightPropertyWidget(Light* light, QWidget *parent = Q_NULLPTR);
    ~LightPropertyWidget();

private:
    Light* light;
    Vector3DWidget * lightPositionWidget, * lightColorWidget;
    AttenuationWidget * attenuationWidget;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void setLightPosition(QVector3D value);
    void setLightColor(QVector3D value);
    void setLightEnableAttenuation(bool enabled);
    void setLightAttenuationQuadratic(double value);
    void setLightAttenuationLinear(double value);
    void setLightAttenuationConstant(double value);
};


class AttenuationWidget: public QGroupBox {
    Q_OBJECT

public:
    AttenuationWidget(QWidget *parent = Q_NULLPTR);
    ~AttenuationWidget();

    void setEnableAttenuation(bool enabled);
    void setQuadraticValue(float value);
    void setLinearValue(float value);
    void setConstantValue(float value);

private:
    QCheckBox * enableAttenuationCheckBox;
    QLabel * quadraticLabel, *linearLabel, *constantLabel;
    QDoubleSpinBox * quadraticSpinBox, *linearSpinBox, *constantSpinBox;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

signals:
    void enableAttenuationChanged(bool enabled);
    void quadraticValueChanged(double value);
    void linearValueChanged(double value);
    void constantValueChanged(double value);

private slots:
    void enableAttenuationCheckBoxStateChanged(int state);
};
