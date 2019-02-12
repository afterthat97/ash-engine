#include <AbstractEntity.h>

AbstractEntity* AbstractEntity::m_highlightedObject = 0;
AbstractEntity* AbstractEntity::m_selectedObject = 0;

AbstractEntity::AbstractEntity(QObject * parent): QObject(0) {
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
    resetTransformation();
    setParent(parent);
}

AbstractEntity::AbstractEntity(const AbstractEntity & abstractObject3D): QObject(0) {
    setObjectName(abstractObject3D.objectName());
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
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

    model.translate(m_position);
    model.rotate(QQuaternion::fromEulerAngles(m_rotation));
    model.scale(m_scaling);

    return model;
}

QMatrix4x4 AbstractEntity::localModelMatrix(bool includeTranslation, bool includeRotation, bool includeScaling) const {
    QMatrix4x4 model;

    if (includeTranslation)
        model.translate(m_position);
    if (includeRotation)
        model.rotate(QQuaternion::fromEulerAngles(m_rotation));
    if (includeScaling)
        model.scale(m_scaling);

    return model;
}

QMatrix4x4 AbstractEntity::globalModelMatrix() const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->globalModelMatrix()
        * localModelMatrix();
    else
        return localModelMatrix();
}

QMatrix4x4 AbstractEntity::globalModelMatrix(bool includeTranslation, bool includeRotation, bool includeScaling) const {
    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->globalModelMatrix()
        * localModelMatrix(includeTranslation, includeRotation, includeScaling);
    else
        return localModelMatrix(includeTranslation, includeRotation, includeScaling);
}

AbstractEntity * AbstractEntity::getHighlighted() {
    return m_highlightedObject;
}

AbstractEntity * AbstractEntity::getSelected() {
    return m_selectedObject;
}

void AbstractEntity::resetTransformation() {
    setPosition(QVector3D());
    setRotation(QVector3D());
    setScaling(QVector3D(1.0f, 1.0f, 1.0f));
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

#ifdef _DEBUG
    dout << this->objectName() << "is" << (selected ? "selected" : "deselected");
#endif

    m_selected = selected;
    selectedChanged(m_selected);
}

void AbstractEntity::setPosition(QVector3D position) {
    if (isnan(position.x()) || isnan(position.y()) || isnan(position.z())) {
#ifdef _DEBUG
        dout << "Failed to set position: NaN detected";
#endif
        return;
    }

    if (!qFuzzyCompare(m_position, position)) {
        m_position = position;
        positionChanged(m_position);
    }
}

void AbstractEntity::setRotation(QQuaternion rotation) {
    setRotation(rotation.toEulerAngles());
}

void AbstractEntity::setRotation(QVector3D rotation) {
    if (isnan(rotation.x()) || isnan(rotation.y()) || isnan(rotation.z())) {
#ifdef _DEBUG
        dout << "Failed to set rotation: NaN detected";
#endif
        return;
    }

    if (!qFuzzyCompare(m_rotation, rotation)) {
        m_rotation = rotation;
        rotationChanged(m_rotation);
    }
}

void AbstractEntity::setScaling(QVector3D scaling) {
    if (isnan(scaling.x()) || isnan(scaling.y()) || isnan(scaling.z())) {
#ifdef _DEBUG
        dout << "Failed to set scaling: NaN detected";
#endif
        return;
    }

    if (!qFuzzyCompare(m_scaling, scaling)) {
        m_scaling = scaling;
        scalingChanged(m_scaling);
    }
}
