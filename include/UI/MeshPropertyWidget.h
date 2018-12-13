#pragma once

#include <Generic/Mesh.h>
#include <UI/Vector3DWidget.h>

class MeshPropertyWidget: public QWidget {
    Q_OBJECT

public:
    MeshPropertyWidget(Mesh* mesh, QWidget *parent = Q_NULLPTR);
    ~MeshPropertyWidget();

private:
    Mesh * mesh;
    Vector3DWidget * meshPositionWidget, *meshRotationWidget, *meshScalingWidget;

    void setupLayout();
    void setupSignals();

private slots:
    void setMeshPosition(QVector3D value);
    void setMeshRotation(QVector3D value);
    void setMeshScaling(QVector3D value);
};