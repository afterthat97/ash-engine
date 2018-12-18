#pragma once

#include <Generic/Mesh.h>

class Model: public QObject {
    Q_OBJECT

public:
    Model(QObject * parent = 0);
    ~Model();
    
    bool isVisible();
    bool isTopLevel();

    QVector3D getLocalPosition();
    QVector3D getLocalRotation();
    QVector3D getLocalScaling();
    QMatrix4x4 getLocalModelMatrix();

    QMatrix4x4 getGlobalModelMatrix();
    QVector3D getGlobalPosition();

    QVector3D getCenterOfMass();
    float getMass();

    vector<Mesh*> getMeshes();
    vector<Model*> getChildren();

    void translate(QVector3D delta);
    void rotate(QQuaternion rotation);
    void rotate(QVector3D rotation);
    void scale(QVector3D scaling);

public slots:
    void setVisible(bool visible);
    void setTopLevel(bool topLevel);

    void resetTransformation();
    void resetChildrenTransformation();
    void setPosition(QVector3D position);
    void setRotation(QQuaternion rotation);
    void setRotation(QVector3D rotation);
    void setScaling(QVector3D scaling);

    void addMesh(Mesh* newMesh);
    void addChildren(Model* newModel);

signals:
    void positionChanged(QVector3D position);
    void rotationChanged(QVector3D rotation);
    void scalingChanged(QVector3D scaling);

private:
    bool visible, topLevel;
    QVector3D position, rotation, scaling;

    vector<Mesh*> meshes;
    vector<Model*> children;
};