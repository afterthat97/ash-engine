#include <AbstractEntity.h>
#include <AbstractGizmo.h>
#include <AbstractLight.h>
#include <Model.h>

AbstractEntity* AbstractEntity::m_highlightedObject = 0;
AbstractEntity* AbstractEntity::m_selectedObject = 0;

AbstractEntity::AbstractEntity(QObject * parent): QObject(0) {
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
    m_wireFrameMode = false;
    m_position = QVector3D(0, 0, 0);
    m_rotation = QVector3D(0, 0, 0);
    m_scaling = QVector3D(1, 1, 1);
    setParent(parent);
}

AbstractEntity::AbstractEntity(const AbstractEntity & abstractObject3D): QObject(0) {
    setObjectName(abstractObject3D.objectName());
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
    m_wireFrameMode = abstractObject3D.m_wireFrameMode;
    m_position = abstractObject3D.m_position;
    m_rotation = abstractObject3D.m_rotation;
    m_scaling = abstractObject3D.m_scaling;
}

AbstractEntity::~AbstractEntity() {
    setHighlighted(false);
    setSelected(false);
}

void AbstractEntity::translate(QVector3D delta) {
    setPosition(m_position + delta);
}

void AbstractEntity::rotate(QQuaternion rotation) {
    setRotation(QQuaternion::fromEulerAngles(m_rotation) * rotation);
}

void AbstractEntity::rotate(QVector3D rotation) {
    setRotation(QQuaternion::fromEulerAngles(m_rotation) * QQuaternion::fromEulerAngles(rotation));
}

void AbstractEntity::scale(QVector3D scaling) {
    setScaling(m_scaling * scaling);
}

bool AbstractEntity::visible() const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->visible() && m_visible;
    else
        return m_visible;
}

bool AbstractEntity::highlighted() const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->highlighted() || m_highlighted;
    else
        return m_highlighted;
}

bool AbstractEntity::selected() const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->selected() || m_selected;
    else
        return m_selected;
}

bool AbstractEntity::wireFrameMode() const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->wireFrameMode() || m_wireFrameMode;
    else
        return m_wireFrameMode;
}

QVector3D AbstractEntity::position() const {
    return m_position;
}

QVector3D AbstractEntity::rotation() const {
    return m_rotation;
}

QVector3D AbstractEntity::scaling() const {
    return m_scaling;
}

QMatrix4x4 AbstractEntity::localModelMatrix() const {
    QMatrix4x4 model;

    model.translate(position());
    model.rotate(QQuaternion::fromEulerAngles(rotation()));
    model.scale(scaling());

    return model;
}

QMatrix4x4 AbstractEntity::globalModelMatrix() const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->globalModelMatrix() * localModelMatrix();
    else
        return localModelMatrix();
}

AbstractEntity * AbstractEntity::getHighlighted() {
    return m_highlightedObject;
}

AbstractEntity * AbstractEntity::getSelected() {
    return m_selectedObject;
}

void AbstractEntity::setVisible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        visibleChanged(m_visible);
    }
}

void AbstractEntity::setHighlighted(bool highlighted) {
    if (m_highlighted == highlighted)
        return;

    if (highlighted) {
        if (m_highlightedObject && m_highlightedObject != this)
            m_highlightedObject->setHighlighted(false);
        m_highlightedObject = this;
    } else if (m_highlightedObject == this)
        m_highlightedObject = 0;

    m_highlighted = highlighted;
    highlightedChanged(m_highlighted);
}

void AbstractEntity::setSelected(bool selected) {
    if (m_selected == selected)
        return;

    if (selected) {
        if (m_selectedObject && m_selectedObject != this)
            m_selectedObject->setSelected(false);
        m_selectedObject = this;
    } else if (m_selectedObject == this)
        m_selectedObject = 0;

#ifdef DEBUG_OUTPUT
    dout << this->objectName() << "is" << (selected ? "selected" : "deselected");
#endif

    m_selected = selected;
    selectedChanged(m_selected);
}

void AbstractEntity::setWireFrameMode(bool enabled) {
    if (m_wireFrameMode != enabled) {
        m_wireFrameMode = enabled;
        wireFrameModeChanged(m_wireFrameMode);
    }
}

void AbstractEntity::setPosition(QVector3D position) {
    if (isnan(position)) {
#ifdef DEBUG_OUTPUT
        dout << "Failed to set position: NaN detected";
#endif
        return;
    }

    if (!isEqual(m_position, position)) {
        m_position = position;
        positionChanged(m_position);
    }
}

void AbstractEntity::setRotation(QQuaternion rotation) {
    setRotation(rotation.toEulerAngles());
}

void AbstractEntity::setRotation(QVector3D rotation) {
    if (isnan(rotation)) {
#ifdef DEBUG_OUTPUT
        dout << "Failed to set rotation: NaN detected";
#endif
        return;
    }

    if (!isEqual(m_rotation, rotation)) {
        m_rotation = rotation;
        rotationChanged(m_rotation);
    }
}

void AbstractEntity::setScaling(QVector3D scaling) {
    if (isnan(scaling)) {
#ifdef DEBUG_OUTPUT
        dout << "Failed to set scaling: NaN detected";
#endif
        return;
    }

    if (!isEqual(m_scaling, scaling)) {
        m_scaling = scaling;
        scalingChanged(m_scaling);
    }
}
