#pragma once

#include <Generic/Common.h>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector3D tangent;
    QVector2D texCoords;

    Vertex(QVector3D _position = QVector3D(0, 0, 0),
           QVector3D _normal = QVector3D(0, 0, 1),
           QVector3D _tangent = QVector3D(1, 0, 0),
           QVector2D _texCoords = QVector2D(0, 0)) {
        position = _position;
        normal = _normal;
        tangent = _tangent;
        texCoords = _texCoords;
    }
};
