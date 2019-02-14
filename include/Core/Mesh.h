#pragma once

#include <AbstractEntity.h>
#include <Vertex.h>
#include <Material.h>

class ModelLoader;

class Mesh: public AbstractEntity {
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
    ~Mesh();

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;

    bool isGizmo() const override;
    bool isLight() const override;
    bool isMesh() const override;
    bool isModel() const override;

    QVector3D centerOfMass() const;
    float mass() const;

    MeshType meshType() const;
    const QVector<Vertex> & vertices() const;
    const QVector<uint32_t> & indices() const;
    Material* material() const;

    static Mesh* merge(const Mesh* mesh1, const Mesh* mesh2);

public slots:
    void setMeshType(MeshType meshType);
    void setGeometry(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
    bool setMaterial(Material *newMaterial);
    void reverseNormals();
    void reverseTangents();
    void reverseBitangents();

signals:
    void meshTypeChanged(int meshType);
    void geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
    void materialChanged(Material* material);

protected:
    void childEvent(QChildEvent *event) override;

protected:
    MeshType m_meshType;
    QVector<Vertex> m_vertices;
    QVector<uint32_t> m_indices;
    Material *m_material;

    friend ModelLoader;
};

QDataStream &operator>>(QDataStream &in, Mesh::MeshType& meshType);
