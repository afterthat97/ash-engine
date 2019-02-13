#pragma once

#include <QPoint>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>

#define eps (1e-4)
#define inf (1000000.0f)
#define rad(n) ((float) cos((n) * M_PI / 180.0))

bool isEqual(float a, float b);

bool isEqual(QVector3D a, QVector3D b);

bool isnan(QVector3D a);

struct Line {
    QVector3D st, dir;
};

struct Plane {
    QVector3D v, n;
};

Line operator*(const QMatrix4x4 &m, const Line &l);

// Get intersection of a line and a plane:
// L = st + dir * t;
// `p` is a point on the plane and `n` is the normal vector
QVector3D getIntersectionOfLinePlane(Line l, Plane p);

// Get the closest point to L2 on L1:
// L1 = st1 + dir1 * t1
// L2 = st2 + dir2 * t2
QVector3D getClosestPointOfLines(Line l1, Line l2);

// Screen space to world space
Line screenPosToWorldRay(QVector2D cursorPos, QVector2D windowSize, QMatrix4x4 proj, QMatrix4x4 view);