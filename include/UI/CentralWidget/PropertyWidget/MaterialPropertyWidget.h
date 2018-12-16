#pragma once

#include <Generic/Material.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>

class MaterialPropertyWidget: public QWidget {
    Q_OBJECT

public:
    MaterialPropertyWidget(Material* material, QWidget *parent = Q_NULLPTR);
    ~MaterialPropertyWidget();

private:
    Material * material;
    Vector3DEditWidget * ambientColorWidget, *diffuseColorWidget, *specularColorWidget;
    FloatEditWidget * shininessEditWidget;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();
};