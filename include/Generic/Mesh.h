#pragma once

#include <Generic/Vertex.h>
#include <Generic/Material.h>

class Mesh: public Object {
public:
    enum MeshType {
        Triangle,
        Line,
        Point
    };

    Mesh(MeshType meshType = Triangle);
    ~Mesh();

    void setType(MeshType meshType);
    void setVisible(bool visible);
    void setPosition(QVector3D newPosition);
    void setRotation(QVector3D eularAngles);
    void setScaling(QVector3D scalingVector);
    void setVertices(vector<Vertex> newVertices);
    void setIndices(vector<uint32_t> newIndices);
    void setMaterial(Material *newMaterial);

    MeshType getType();
    bool isVisible();
    QVector3D getPosition();
    QVector3D getRotation();
    QVector3D getScaling();
    QMatrix4x4 getModelMatrix();
    vector<Vertex> getVertices();
    vector<uint32_t> getIndices();
    Material* getMaterial();

    void translate(QVector3D delta);
    void rotate(QVector3D eularAngles);
    void scale(QVector3D scalingVector);

private:
    MeshType meshType;
    bool visible;

    QVector3D position;
    QQuaternion rotation;
    QVector3D scaling;

    vector<Vertex> vertices;
    vector<uint32_t> indices;
    Material *material;

    void initID() override;
};