#include <TransformGizmo.h>

TransformGizmo::TransformGizmo(QObject* parent): AbstractGizmo(0) {
    m_translateGizmo = new TranslateGizmo(this);
    m_rotateGizmo = new RotateGizmo(this);
    m_scaleGizmo = new ScaleGizmo(this);
    m_activatedGizmo = m_translateGizmo;
    m_activatedGizmo->setVisible(true);

    for (int i = 0; i < m_translateGizmo->markers().size(); i++)
        m_markers.push_back(m_translateGizmo->markers()[i]);

    for (int i = 0; i < m_rotateGizmo->markers().size(); i++)
        m_markers.push_back(m_rotateGizmo->markers()[i]);

    for (int i = 0; i < m_scaleGizmo->markers().size(); i++)
        m_markers.push_back(m_scaleGizmo->markers()[i]);

    setParent(parent);
}

TransformGizmo::~TransformGizmo() {}

void TransformGizmo::translate(QVector3D delta) {
    if (m_activatedGizmo)
        m_activatedGizmo->translate(delta);
}

void TransformGizmo::rotate(QQuaternion rotation) {
    if (m_activatedGizmo)
        m_activatedGizmo->rotate(rotation);
}

void TransformGizmo::rotate(QVector3D rotation) {
    if (m_activatedGizmo)
        m_activatedGizmo->rotate(rotation);
}

void TransformGizmo::scale(QVector3D scaling) {
    if (m_activatedGizmo)
        m_activatedGizmo->scale(scaling);
}

QVector3D TransformGizmo::position() const {
    if (m_activatedGizmo)
        return m_activatedGizmo->position();
    else
        return QVector3D(0, 0, 0);
}

QVector3D TransformGizmo::rotation() const {
    if (m_activatedGizmo)
        return m_activatedGizmo->rotation();
    else
        return QVector3D(0, 0, 0);
}

QVector3D TransformGizmo::scaling() const {
    if (m_activatedGizmo)
        return m_activatedGizmo->scaling();
    else
        return QVector3D(1, 1, 1);
}

QMatrix4x4 TransformGizmo::globalModelMatrix() const {
    if (m_activatedGizmo)
        return m_activatedGizmo->globalModelMatrix();
    else
        return QMatrix4x4();
}

TransformGizmo::TransformAxis TransformGizmo::transformAxis() const {
    if (m_activatedGizmo)
        return m_activatedGizmo->transformAxis();
    else
        return None;
}

TransformGizmo::TransformMode TransformGizmo::transformMode() const {
    if (m_activatedGizmo == m_translateGizmo)
        return Translate;
    if (m_activatedGizmo == m_rotateGizmo)
        return Rotate;
    return Scale;
}

QVector<Mesh*>& TransformGizmo::markers() {
    return m_markers;
}

void TransformGizmo::drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view) {
    if (m_activatedGizmo)
        m_activatedGizmo->drag(from, to, scnWidth, scnHeight, proj, view);
}

void TransformGizmo::bindTo(AbstractEntity * host) {
    AbstractGizmo::bindTo(host);
    m_translateGizmo->bindTo(host);
    m_rotateGizmo->bindTo(host);
    m_scaleGizmo->bindTo(host);
    m_visible = (host != 0);
}

void TransformGizmo::unbind() {
    AbstractGizmo::unbind();
    m_translateGizmo->unbind();
    m_rotateGizmo->unbind();
    m_scaleGizmo->unbind();
    m_visible = false;
}

void TransformGizmo::setTransformAxis(TransformAxis axis) {
    if (m_activatedGizmo)
        m_activatedGizmo->setTransformAxis(axis);
}

void TransformGizmo::setTransformMode(TransformMode mode) {
    m_activatedGizmo->setVisible(false);
    if (mode == Translate) {
        m_activatedGizmo = m_translateGizmo;
    } else if (mode == Rotate) {
        m_activatedGizmo = m_rotateGizmo;
    } else if (mode == Scale) {
        m_activatedGizmo = m_scaleGizmo;
    }
    m_activatedGizmo->setVisible(true);
}

void TransformGizmo::setPosition(QVector3D position) {
    if (m_activatedGizmo)
        m_activatedGizmo->setPosition(position);
}

void TransformGizmo::setRotation(QQuaternion rotation) {
    if (m_activatedGizmo)
        m_activatedGizmo->setRotation(rotation);
}

void TransformGizmo::setRotation(QVector3D rotation) {
    if (m_activatedGizmo)
        m_activatedGizmo->setRotation(rotation);
}

void TransformGizmo::setScaling(QVector3D scaling) {
    if (m_activatedGizmo)
        m_activatedGizmo->setScaling(scaling);
}
