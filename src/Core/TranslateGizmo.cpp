#include <TranslateGizmo.h>
#include <ModelLoader.h>

TranslateGizmo::TranslateGizmo(QObject* parent): AbstractGizmo(0) {
    setObjectName("Translation Gizmo");
    m_markers.resize(3);

    ModelLoader loader;
    m_markers[0] = loader.loadMeshFromFile(":/resources/shapes/TransX.obj");
    m_markers[1] = loader.loadMeshFromFile(":/resources/shapes/TransY.obj");
    m_markers[2] = loader.loadMeshFromFile(":/resources/shapes/TransZ.obj");

    m_markers[0]->material()->setColor(QVector3D(1, 0, 0));
    m_markers[1]->material()->setColor(QVector3D(0, 1, 0));
    m_markers[2]->material()->setColor(QVector3D(0, 0, 1));

    for (int i = 0; i < m_markers.size(); i++)
        m_markers[i]->setParent(this);

    setParent(parent);
}

TranslateGizmo::~TranslateGizmo() {}

void TranslateGizmo::translate(QVector3D delta) {
    if (m_host)
        m_host->translate(delta);
}

void TranslateGizmo::rotate(QQuaternion) {
#ifdef _DEBUG
    dout << "Trying to rotate a TRANSLATION ONLY gizmo is not allowed.";
#endif
}

void TranslateGizmo::rotate(QVector3D) {
#ifdef _DEBUG
    dout << "Trying to rotate a TRANSLATION ONLY gizmo is not allowed.";
#endif
}

void TranslateGizmo::scale(QVector3D) {
#ifdef _DEBUG
    dout << "Trying to scale a TRANSLATION ONLY gizmo is not allowed.";
#endif
}

QVector3D TranslateGizmo::position() const {
    if (m_host)
        return m_host->position();
    else
        return QVector3D(0, 0, 0);
}

QVector3D TranslateGizmo::rotation() const {
    return QVector3D(0, 0, 0);
}

QVector3D TranslateGizmo::scaling() const {
    return QVector3D(1, 1, 1);
}

QMatrix4x4 TranslateGizmo::globalSpaceMatrix() const {
    QMatrix4x4 model = localModelMatrix();

    if (m_host) {
        AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
        while (parent) {
            model = parent->globalModelMatrix() * model;
            parent = qobject_cast<AbstractEntity*>(parent->parent());
        }
    }

    return model;
}

QMatrix4x4 TranslateGizmo::globalModelMatrix() const {
    QMatrix4x4 model;
    model.translate(globalSpaceMatrix() * QVector3D(0, 0, 0));

    if (m_host) {
        QQuaternion globalRotation;
        AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
        while (parent) {
            globalRotation = QQuaternion::fromEulerAngles(parent->rotation()) * globalRotation;
            parent = qobject_cast<AbstractEntity*>(parent->parent());
        }
        model.rotate(globalRotation);
    }

    return model;
}

void TranslateGizmo::drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view) {
    if (m_host == 0) return;
    Line l1 = screenPosToWorldRay(QVector2D(from), QVector2D(scnWidth, scnHeight), proj, view);
    Line l2 = screenPosToWorldRay(QVector2D(to), QVector2D(scnWidth, scnHeight), proj, view);
    QMatrix4x4 invModelMat = globalSpaceMatrix().inverted();
    l1 = invModelMat * l1;
    l2 = invModelMat * l2;
    if (m_axis == TranslateGizmo::X) {
        Line x = { QVector3D(0, 0, 0), QVector3D(1, 0, 0) };
        QVector3D p1 = getClosestPointOfLines(x, l1);
        QVector3D p2 = getClosestPointOfLines(x, l2);
        translate(p2 - p1);
    } else if (m_axis == TranslateGizmo::Y) {
        Line y = { QVector3D(0, 0, 0), QVector3D(0, 1, 0) };
        QVector3D p1 = getClosestPointOfLines(y, l1);
        QVector3D p2 = getClosestPointOfLines(y, l2);
        translate(p2 - p1);
    } else if (m_axis == TranslateGizmo::Z) {
        Line z = { QVector3D(0, 0, 0), QVector3D(0, 0, 1) };
        QVector3D p1 = getClosestPointOfLines(z, l1);
        QVector3D p2 = getClosestPointOfLines(z, l2);
        translate(p2 - p1);
    }
}

void TranslateGizmo::setPosition(QVector3D position) {
    if (m_host)
        m_host->setPosition(position);
}

void TranslateGizmo::setRotation(QQuaternion) {
#ifdef _DEBUG
    dout << "Trying to rotate a TRANSLATION ONLY gizmo is not allowed.";
#endif
}

void TranslateGizmo::setRotation(QVector3D) {
#ifdef _DEBUG
    dout << "Trying to rotate a TRANSLATION ONLY gizmo is not allowed.";
#endif
}

void TranslateGizmo::setScaling(QVector3D) {
#ifdef _DEBUG
    dout << "Trying to scale a TRANSLATION ONLY gizmo is not allowed.";
#endif
}
