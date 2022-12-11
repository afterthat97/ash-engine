#include <extmath.h>
#include <Common.h>
#include <math.h>

bool isEqual(float a, float b) {
    return qAbs(a - b) < eps;
}

bool isEqual(QVector3D a, QVector3D b) {
    return isEqual(a[0], b[0]) && isEqual(a[1], b[1]) && isEqual(a[2], b[2]);
}

bool isnan(QVector2D a) {
    return qIsNaN(a[0]) || qIsNaN(a[1]);
}

bool isnan(QVector3D a) {
    return qIsNaN(a[0]) || qIsNaN(a[1]) || qIsNaN(a[2]);
}

bool isnan(QVector4D a) {
    return qIsNaN(a[0]) || qIsNaN(a[1]) || qIsNaN(a[2]) || qIsNaN(a[3]);
}

Line operator*(const QMatrix4x4 &m, const Line &l) {
    QVector3D st = l.st, ed = l.st + l.dir;
    st = m * st;
    ed = m * ed;
    return { st, ed - st };
}

QVector3D getIntersectionOfLinePlane(Line l, Plane p) {
    float t = QVector3D::dotProduct(p.n, p.v - l.st) / QVector3D::dotProduct(p.n, l.dir);
    if (isnan(t) && log_level >= LOG_LEVEL_WARNING)
        dout << "Warning: NaN detected";
    return l.st + l.dir * t;
}

QVector3D getClosestPointOfLines(Line l1, Line l2) {
    l1.dir.normalize();
    l2.dir.normalize();
    QVector3D n = QVector3D::crossProduct(l1.dir, l2.dir);
    if (isnan(n) && log_level >= LOG_LEVEL_WARNING)
        dout << "Warning: NaN detected";
    return getIntersectionOfLinePlane({ l1.st, l1.dir },
                                      { l2.st, QVector3D::crossProduct(n, l2.dir) });
}

Line screenPosToWorldRay(QVector2D cursorPos, QVector2D windowSize, QMatrix4x4 proj, QMatrix4x4 view) {
    cursorPos.setY(windowSize.y() - cursorPos.y());
    QMatrix4x4 inv = (proj * view).inverted();
    QVector4D st((cursorPos / windowSize - QVector2D(0.5, 0.5)) * 2.0f, -1.0, 1.0f);
    QVector4D ed((cursorPos / windowSize - QVector2D(0.5, 0.5)) * 2.0f, 0.0, 1.0f);
    st = inv * st; st /= st.w();
    ed = inv * ed; ed /= ed.w();
    if (isnan(st) && log_level >= LOG_LEVEL_WARNING)
        dout << "Warning: NaN detected";
    if (isnan(ed) && log_level >= LOG_LEVEL_WARNING)
        dout << "Warning: NaN detected";
    QVector3D dir = (ed - st).toVector3D();
    if (dir.isNull() && log_level >= LOG_LEVEL_WARNING)
        dout << "Warning: dir is null";
    return Line { st.toVector3D(), dir.normalized() };
}
