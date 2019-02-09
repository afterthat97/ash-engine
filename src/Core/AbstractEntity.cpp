#include <AbstractEntity.h>

AbstractEntity* AbstractEntity::m_highlightedObject = 0;
AbstractEntity* AbstractEntity::m_selectedObject = 0;

AbstractEntity::AbstractEntity(QObject * parent): QObject(0) {
    m_visible = true;
    m_highlighted = false;
    m_selected = false;
    m_canTranslate = true;
    m_canRotate = true;
    m_canScale = true;
    m_host = 0;
    resetTransformation();
    setParent(parent);
}

AbstractEntity::AbstractEntity(const AbstractEntity & abstractObject3D): QObject(0) {
    setObjectName(abstractObject3D.objectName());
    m_visible = abstractObject3D.m_visible;
    m_highlighted = false;
    m_selected = false;
    m_canTranslate = abstractObject3D.m_canTranslate;
    m_canRotate = abstractObject3D.m_canRotate;
    m_canScale = abstractObject3D.m_canScale;
    m_position = abstractObject3D.m_position;
    m_rotation = abstractObject3D.m_rotation;
    m_scaling = abstractObject3D.m_scaling;
    m_host = 0;
}

AbstractEntity::~AbstractEntity() {
    setHighlighted(false);
    setSelected(false);
}

void AbstractEntity::translate(QVector3D delta) {
    if (m_host)
        m_host->translate(delta);
    else
        setPosition(m_position + delta);
}

void AbstractEntity::rotate(QQuaternion rotation) {
    if (m_host)
        m_host->rotate(rotation);
    else
        setRotation(QQuaternion::fromEulerAngles(m_rotation) * rotation);
}

void AbstractEntity::rotate(QVector3D rotation) {
    if (m_host)
        m_host->rotate(rotation);
    else
        setRotation(QQuaternion::fromEulerAngles(m_rotation) * QQuaternion::fromEulerAngles(rotation));
}

void AbstractEntity::scale(QVector3D scaling) {
    if (m_host)
        m_host->scale(scaling);
    else
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

bool AbstractEntity::canTranslate() const {
    return m_canTranslate;
}

bool AbstractEntity::canRotate() const {
    return m_canRotate;
}

bool AbstractEntity::canScale() const {
    return m_canScale;
}

QVector3D AbstractEntity::position() const {
    if (m_host)
        return m_host->position();
    else
        return m_position;
}

QVector3D AbstractEntity::rotation() const {
    if (m_host)
        return m_host->rotation();
    else
        return m_rotation;
}

QVector3D AbstractEntity::scaling() const {
    if (m_host)
        return m_host->scaling();
    else
        return m_scaling;
}

QMatrix4x4 AbstractEntity::localModelMatrix() const {
    if (m_host)
        return m_host->localModelMatrix();

    QMatrix4x4 model;
    if (m_canTranslate)
        model.translate(m_position);
    if (m_canRotate)
        model.rotate(QQuaternion::fromEulerAngles(m_rotation));
    if (m_canScale)
        model.scale(m_scaling);

    return model;
}

QMatrix4x4 AbstractEntity::localModelMatrix(bool includeTranslation, bool includeRotation, bool includeScaling) const {
    if (m_host)
        return m_host->localModelMatrix(includeTranslation, includeRotation, includeScaling);

    QMatrix4x4 model;
    if (includeTranslation && m_canTranslate)
        model.translate(m_position);
    if (includeRotation && m_canRotate)
        model.rotate(QQuaternion::fromEulerAngles(m_rotation));
    if (includeScaling && m_canScale)
        model.scale(m_scaling);

    return model;
}

QMatrix4x4 AbstractEntity::globalModelMatrix() const {
    if (m_host)
        return m_host->globalModelMatrix();
    else if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->globalModelMatrix(m_canTranslate, m_canRotate, m_canScale)
        * localModelMatrix();
    else
        return localModelMatrix();
}

QMatrix4x4 AbstractEntity::globalModelMatrix(bool includeTranslation, bool includeRotation, bool includeScaling) const {
    if (m_host)
        return m_host->globalModelMatrix(includeTranslation, includeRotation, includeScaling);

    includeTranslation = includeTranslation && m_canTranslate;
    includeRotation = includeRotation && m_canRotate;
    includeScaling = includeScaling && m_canScale;

    if (AbstractEntity* par = qobject_cast<AbstractEntity*>(parent()))
        return par->globalModelMatrix(includeTranslation, includeRotation, includeScaling)
        * localModelMatrix(includeTranslation, includeRotation, includeScaling);
    else
        return localModelMatrix(includeTranslation, includeRotation, includeScaling);
}

AbstractEntity * AbstractEntity::host() const {
    return m_host;
}

void AbstractEntity::bindTo(AbstractEntity * host) {
    if (m_host)
        unbind();
    if (host) {
        m_host = host;
        connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
#ifdef _DEBUG
        dout << this->objectName() << "is bound to" << host->objectName();
#endif
    }
}

void AbstractEntity::unbind() {
    if (m_host) {
        disconnect(m_host, 0, this, 0);
        m_host = 0;
#ifdef _DEBUG
        dout << this->objectName() << "is unbound";
#endif
    }
}

void AbstractEntity::hostDestroyed(QObject * host) {
    if (m_host == host)
        unbind();
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

void AbstractEntity::setTransformOptions(bool canTranslate, bool canRotate, bool canScale) {
    if (m_host) {
        m_host->setTransformOptions(canTranslate, canRotate, canScale);
        return;
    }

    m_canTranslate = canTranslate;
    m_canRotate = canRotate;
    m_canScale = canScale;
}

void AbstractEntity::setPosition(QVector3D position) {
    if (m_host) {
        m_host->setPosition(position);
        return;
    }

    if (!m_canTranslate) {
#ifdef _DEBUG
        dout << "Failed to set position: Translation is not allowed";
#endif
        return;
    }

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
    if (m_host) {
        m_host->setRotation(rotation);
        return;
    }

    if (!m_canRotate) {
#ifdef _DEBUG
        dout << "Failed to set rotation: Rotation is not allowed";
#endif
        return;
    }

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
    if (m_host) {
        m_host->setScaling(scaling);
        return;
    }

    if (!m_canScale) {
#ifdef _DEBUG
        dout << "Failed to set scaling: Scaling is not allowed";
#endif
        return;
    }

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
