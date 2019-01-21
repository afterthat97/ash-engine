#pragma once

#include <Core/Common.h>

class Vertex {
public:
    Vertex(QVector3D _position = QVector3D(0, 0, 0),
           QVector3D _normal = QVector3D(0, 0, 1),
           QVector3D _tangent = QVector3D(0, 1, 0),
           QVector3D _bitangent = QVector3D(1, 0, 0),
           QVector2D _texCoords = QVector2D(0, 0));
    Vertex(const Vertex &other);
    Vertex& operator=(const Vertex &other);
    bool operator==(const Vertex &other) const;

    QVector3D position;
    QVector3D normal;
    QVector3D tangent;
    QVector3D bitangent;
    QVector2D texCoords;
};

QDataStream &operator<<(QDataStream &out, const Vertex& vertex);
QDataStream &operator>>(QDataStream &in, Vertex& vertex);
