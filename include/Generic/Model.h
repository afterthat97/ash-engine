#pragma once

#include <Generic/Mesh.h>

class Model: public QObject {
    Q_OBJECT

public:
    Model(QObject * parent = 0);
    ~Model();
    
    bool isVisible();

    QVector3D getPosition();
    QVector3D getRotation();
    QVector3D getScaling();

    vector<Mesh*> getMeshes();
    vector<Model*> getChildren();

    void translate(QVector3D delta);
    void rotate(QVector3D delta);
    void scale(QVector3D delta);

public slots:
    void setVisible(bool visible);

    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    void setScaling(QVector3D scaling);

    void addMesh(Mesh* newMesh);
    void addChildren(Model* newModel);

signals:
    void positionChanged(QVector3D position);
    void rotationChanged(QVector3D rotation);
    void scalingChanged(QVector3D scaling);

private:
    bool visible;
    QVector3D position, rotation, scaling;

    vector<Mesh*> meshes;
    vector<Model*> children;
};