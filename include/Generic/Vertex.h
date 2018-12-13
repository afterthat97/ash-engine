#pragma once

#include <Generic/Common.h>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector3D tangent;
    QVector2D texCoords;

    Vertex() {}
    Vertex(QVector3D _position) {
        position = _position;
        normal = QVector3D(1.0f, 0.0f, 0.0f);
        tangent = QVector3D(0.0f, 1.0f, 0.0f);
        texCoords = QVector2D(0.0f, 0.0f);
    }
    Vertex(QVector3D _position, QVector3D _normal, QVector3D _tangent, QVector2D _texCoords) {
        position = _position;
        normal = _normal;
        tangent = _tangent;
        texCoords = _texCoords;
    }
};
