#pragma once

#include <Core/Vertex.h>

class Material;

class Mesh: public QObject {
    Q_OBJECT

public:
    enum MeshType {
        Triangle = 0,
        Line = 1,
        Point = 2
    };

    Mesh(QObject * parent = 0);
    Mesh(MeshType meshType, QObject * parent = 0);
    Mesh(const Mesh& mesh);

    void translate(QVector3D delta);
    void rotate(QQuaternion rotation);
    void rotate(QVector3D rotation);
    void scale(QVector3D scaling);

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);
    
    QVector3D centerOfMass() const;
    float mass() const;

    bool visible() const;
    MeshType meshType() const;
    QVector3D localPosition() const;
    QVector3D globalPosition() const;
    QVector3D localRotation() const;
    QVector3D localScaling() const;
    QMatrix4x4 localModelMatrix() const;
    QMatrix4x4 globalModelMatrix() const;
    const QVector<Vertex> & vertices() const;
    const QVector<uint32_t> & indices() const;
    Material* material() const;

public slots:
    void resetTransformation();
    void setVisible(bool visible);
    void setMeshType(MeshType meshType);
    void setPosition(QVector3D position);
    void setRotation(QQuaternion rotation);
    void setRotation(QVector3D rotation);
    void setScaling(QVector3D scaling);
    void setGeometry(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
    void setMaterial(Material *newMaterial);

signals:
    void visibleChanged(bool visible);
    void meshTypeChanged(MeshType meshType);
    void positionChanged(QVector3D position);
    void rotationChanged(QVector3D rotation);
    void scalingChanged(QVector3D scaling);
    void geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
    void materialChanged(Material* material);

protected:
    void childEvent(QChildEvent *event) override;

private:
    bool m_visible;
    MeshType m_meshType;
    QVector3D m_position, m_rotation, m_scaling;
    QVector<Vertex> m_vertices;
    QVector<uint32_t> m_indices;
    Material *m_material;
};

QDataStream &operator>>(QDataStream &in, Mesh::MeshType& meshType);