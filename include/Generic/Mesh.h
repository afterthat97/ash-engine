#pragma once

#include <Generic/Vertex.h>
#include <Generic/Material.h>

class Mesh: public QObject {
    Q_OBJECT

public:
    enum MeshType {
        Triangle,
        Line,
        Point
    };

    Mesh(MeshType meshType = Triangle, QObject * parent = 0);
    ~Mesh();

    uint32_t getID();
    MeshType getType();
    bool isVisible();
    bool isNormalReversed();

    QVector3D getLocalPosition();
    QVector3D getLocalRotation();
    QVector3D getLocalScaling();
    QMatrix4x4 getLocalModelMatrix();

    QMatrix4x4 getGlobalModelMatrix();
    QVector3D getGlobalPosition();

    QVector3D getCenterOfMass();
    float getMass();

    vector<Vertex> getVertices();
    vector<uint32_t> getIndices();
    Material* getMaterial();

    void translate(QVector3D delta);
    void rotate(QQuaternion rotation);
    void rotate(QVector3D rotation);
    void scale(QVector3D scaling);

public slots:
    void setVisible(bool visible);
    void setReverseNormal(bool reverseNormal);
    
    void resetTransformation();
    void setPosition(QVector3D position);
    void setRotation(QQuaternion rotation);
    void setRotation(QVector3D rotation);
    void setScaling(QVector3D scaling);

    void setVertices(const vector<Vertex>& vertices);
    void setIndices(const vector<uint32_t>& indices);
    void setMaterial(Material *newMaterial);

signals:
    void positionChanged(QVector3D position);
    void rotationChanged(QVector3D rotation);
    void scalingChanged(QVector3D scaling);

private:
    uint32_t id;
    MeshType meshType;
    bool visible, reverseNormal;
    QVector3D position, rotation, scaling;

    vector<Vertex> vertices;
    vector<uint32_t> indices;
    Material *material;
};