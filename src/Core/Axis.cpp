#include <Core/Axis.h>
#include <IO/ModelLoader.h>

struct Line {
    QVector3D st, dir;
};

Line operator*(const QMatrix4x4 &m, const Line &l) {
    QVector3D st = l.st, ed = l.st + l.dir;
    st = m * st;
    ed = m * ed;
    return { st, ed - st };
}

// Get intersection of a line and a plane:
// L = st + dir * t;
// `p` is a point on the plane and `n` is the normal vector
QVector3D getIntersectionOfLinePlane(Line l, QVector3D p, QVector3D n) {
    float t = QVector3D::dotProduct(n, p - l.st) / QVector3D::dotProduct(n, l.dir);
    return l.st + l.dir * t;
}

// Get the closest point to L2 on L1:
// L1 = st1 + dir1 * t1
// L2 = st2 + dir2 * t2
QVector3D getClosestPointOfLines(Line l1, Line l2) {
    l1.dir.normalize();
    l2.dir.normalize();
    QVector3D n = QVector3D::crossProduct(l1.dir, l2.dir);
    n = QVector3D::dotProduct(l2.st - l1.st, n) * n;
    return getIntersectionOfLinePlane({ l1.st, l1.dir }, l2.st, QVector3D::crossProduct(n, l2.dir));
}

// Screen space to world space
Line screenPosToWorldRay(QVector2D cursorPos, QVector2D windowSize, QMatrix4x4 proj, QMatrix4x4 view) {
    cursorPos.setY(windowSize.y() - cursorPos.y());
    QMatrix4x4 inv = (proj * view).inverted();
    QVector4D st((cursorPos / windowSize - QVector2D(0.5, 0.5)) * 2.0f, -1.0, 1.0f);
    QVector4D ed((cursorPos / windowSize - QVector2D(0.5, 0.5)) * 2.0f, 0.0, 1.0f);
    st = inv * st; st /= st.w();
    ed = inv * ed; ed /= ed.w();
    QVector3D dir = (ed - st).toVector3D();
    dir.normalize();
    return Line{ st.toVector3D(), dir };
}

Axis::Axis(QObject * parent): AbstractEntity(parent) {
    setObjectName("Axis");
    m_visible = false;
    m_markers.resize(9);

    ModelLoader loader;
    m_markers[0] = loader.loadMeshFromFile(":/resources/shapes/TransX.obj");
    m_markers[1] = loader.loadMeshFromFile(":/resources/shapes/TransY.obj");
    m_markers[2] = loader.loadMeshFromFile(":/resources/shapes/TransZ.obj");
    m_markers[3] = loader.loadMeshFromFile(":/resources/shapes/RotX.obj");
    m_markers[4] = loader.loadMeshFromFile(":/resources/shapes/RotY.obj");
    m_markers[5] = loader.loadMeshFromFile(":/resources/shapes/RotZ.obj");
    m_markers[6] = loader.loadMeshFromFile(":/resources/shapes/ScaleX.obj");
    m_markers[7] = loader.loadMeshFromFile(":/resources/shapes/ScaleY.obj");
    m_markers[8] = loader.loadMeshFromFile(":/resources/shapes/ScaleZ.obj");

    m_markers[0]->material()->setColor(QVector3D(1, 0, 0));
    m_markers[1]->material()->setColor(QVector3D(0, 1, 0));
    m_markers[2]->material()->setColor(QVector3D(0, 0, 1));
    m_markers[3]->material()->setColor(QVector3D(1, 0, 0));
    m_markers[4]->material()->setColor(QVector3D(0, 1, 0));
    m_markers[5]->material()->setColor(QVector3D(0, 0, 1));
    m_markers[6]->material()->setColor(QVector3D(1, 0, 0));
    m_markers[7]->material()->setColor(QVector3D(0, 1, 0));
    m_markers[8]->material()->setColor(QVector3D(0, 0, 1));

    m_markers[0]->setTransformOptions(true, false, false);
    m_markers[1]->setTransformOptions(true, false, false);
    m_markers[2]->setTransformOptions(true, false, false);
    m_markers[3]->setTransformOptions(true, true, false);
    m_markers[4]->setTransformOptions(true, true, false);
    m_markers[5]->setTransformOptions(true, true, false);
    m_markers[6]->setTransformOptions(true, true, true);
    m_markers[7]->setTransformOptions(true, true, true);
    m_markers[8]->setTransformOptions(true, true, true);

    for (int i = 0; i < m_markers.size(); i++)
        m_markers[i]->setParent(this);

    setAxisType(Translate);
    setTransformMode(None);
}

Axis::~Axis() {}

void Axis::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Axis: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Host: " << (m_host ? m_host->objectName() : "None");
    qDebug().nospace() << tab(l + 1) << "Type: " <<
        (m_axisType == Translate ? "Translate" : (m_axisType == Rotate ? "Rotate" : "Scale"));
}

void Axis::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

Axis::AxisType Axis::axisType() const {
    return m_axisType;
}

Axis::TransformationMode Axis::transformMode() const {
    return m_mode;
}

QVector<Mesh*>& Axis::markers() {
    return m_markers;
}

void Axis::drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view) {
    if (m_host == 0) return;
    Line l1 = screenPosToWorldRay(QVector2D(from), QVector2D(scnWidth, scnHeight), proj, view);
    Line l2 = screenPosToWorldRay(QVector2D(to),   QVector2D(scnWidth, scnHeight), proj, view);
    QMatrix4x4 invModelMat = m_markers[m_mode - Translate_X]->globalModelMatrix().inverted();
    l1 = invModelMat * l1;
    l2 = invModelMat * l2;
    if (m_mode == Translate_X) {
        Line x = { QVector3D(0, 0, 0), QVector3D(1, 0, 0) };
        QVector3D p1 = getClosestPointOfLines(x, l1);
        QVector3D p2 = getClosestPointOfLines(x, l2);
        translate(p2 - p1);
    } else if (m_mode == Translate_Y) {
        Line y = { QVector3D(0, 0, 0), QVector3D(0, 1, 0) };
        QVector3D p1 = getClosestPointOfLines(y, l1);
        QVector3D p2 = getClosestPointOfLines(y, l2);
        translate(p2 - p1);
    } else if (m_mode == Translate_Z) {
        Line z = { QVector3D(0, 0, 0), QVector3D(0, 0, 1) };
        QVector3D p1 = getClosestPointOfLines(z, l1);
        QVector3D p2 = getClosestPointOfLines(z, l2);
        translate(p2 - p1);
    } else if (m_mode == Rotate_X) {
        QVector3D p1 = getIntersectionOfLinePlane(l1, QVector3D(0, 0, 0), QVector3D(1, 0, 0));
        QVector3D p2 = getIntersectionOfLinePlane(l2, QVector3D(0, 0, 0), QVector3D(1, 0, 0));
        float theta = acos(fmin(fmax(QVector3D::dotProduct(p1, p2) / p1.length() / p2.length(), -1.0f), 1.0f));
        if (QVector3D::dotProduct(QVector3D(1, 0, 0), QVector3D::crossProduct(p1, p2)) < 0)
            theta = -theta;
        rotate(theta * QVector3D(180.0f / float(M_PI), 0.0f, 0.0f));
    } else if (m_mode == Rotate_Y) {
        QVector3D p1 = getIntersectionOfLinePlane(l1, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
        QVector3D p2 = getIntersectionOfLinePlane(l2, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
        float theta = acos(fmin(fmax(QVector3D::dotProduct(p1, p2) / p1.length() / p2.length(), -1.0f), 1.0f));
        if (QVector3D::dotProduct(QVector3D(0, 1, 0), QVector3D::crossProduct(p1, p2)) < 0)
            theta = -theta;
        rotate(theta * QVector3D(0.0f, 180.0f / float(M_PI), 0.0f));
    } else if (m_mode == Rotate_Z) {
        QVector3D p1 = getIntersectionOfLinePlane(l1, QVector3D(0, 0, 0), QVector3D(0, 0, 1));
        QVector3D p2 = getIntersectionOfLinePlane(l2, QVector3D(0, 0, 0), QVector3D(0, 0, 1));
        float theta = acos(fmin(fmax(QVector3D::dotProduct(p1, p2) / p1.length() / p2.length(), -1.0f), 1.0f));
        if (QVector3D::dotProduct(QVector3D(0, 0, 1), QVector3D::crossProduct(p1, p2)) < 0)
            theta = -theta;
        rotate(theta * QVector3D(0.0f, 0.0f, 180.0f / float(M_PI)));
    } else if (m_mode == Scale_X) {
        Line x = { QVector3D(0, 0, 0), QVector3D(1, 0, 0) };
        QVector3D p1 = getClosestPointOfLines(x, l1);
        QVector3D p2 = getClosestPointOfLines(x, l2);
        scale(QVector3D(p2.x() / p1.x(), 1.0f, 1.0f));
    } else if (m_mode == Scale_Y) {
        Line y = { QVector3D(0, 0, 0), QVector3D(0, 1, 0) };
        QVector3D p1 = getClosestPointOfLines(y, l1);
        QVector3D p2 = getClosestPointOfLines(y, l2);
        scale(QVector3D(1.0f, p2.y() / p1.y(), 1.0f));
    } else if (m_mode == Scale_Z) {
        Line z = { QVector3D(0, 0, 0), QVector3D(0, 0, 1) };
        QVector3D p1 = getClosestPointOfLines(z, l1);
        QVector3D p2 = getClosestPointOfLines(z, l2);
        scale(QVector3D(1.0f, 1.0f, p2.z() / p1.z()));
    }
}

void Axis::bindTo(AbstractEntity * host) {
    AbstractEntity::bindTo(host);
    m_visible = (m_host != 0);
}

void Axis::unbind() {
    AbstractEntity::unbind();
    m_visible = 0;
}

void Axis::setAxisType(AxisType axisType) {
    for (int i = 0; i < 9; i++)
        m_markers[i]->setVisible(false);
    m_axisType = axisType;
    for (int i = 0; i < 3; i++)
        m_markers[i + m_axisType]->setVisible(true);
}

void Axis::setTransformMode(TransformationMode mode) {
    m_mode = mode;
}

