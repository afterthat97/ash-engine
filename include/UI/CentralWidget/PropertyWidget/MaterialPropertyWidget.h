#pragma once

#include <Generic/Material.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DWidget.h>

class ShininessWidget;

class MaterialPropertyWidget: public QWidget {
    Q_OBJECT

public:
    MaterialPropertyWidget(Material* material, QWidget *parent = Q_NULLPTR);
    ~MaterialPropertyWidget();

private:
    Material * material;
    Vector3DWidget * ambientColorWidget, *diffuseColorWidget, *specularColorWidget;
    ShininessWidget * shininessWidget;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void setAmbientColor(QVector3D value);
    void setDiffuseColor(QVector3D value);
    void setSpecularColor(QVector3D value);
    void setShininess(double value);
};

class ShininessWidget: public QGroupBox {
    Q_OBJECT

public:
    ShininessWidget(QWidget *parent = Q_NULLPTR);
    ~ShininessWidget();

    void setShininess(float value);

private:
    QLabel * label;
    QDoubleSpinBox * spinBox;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

signals:
    void valueChanged(double value);
};