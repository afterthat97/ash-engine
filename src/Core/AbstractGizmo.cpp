#include <AbstractGizmo.h>

AbstractGizmo::AbstractGizmo(QObject* parent): AbstractEntity(0) {
    m_visible = false;
    m_axis = None;
    m_host = 0;
    setParent(parent);
}

AbstractGizmo::~AbstractGizmo() {}

bool AbstractGizmo::isGizmo() const {
    return true;
}

bool AbstractGizmo::isLight() const {
    return false;
}

bool AbstractGizmo::isMesh() const {
    return false;
}

bool AbstractGizmo::isModel() const {
    return false;
}

AbstractGizmo::TransformAxis AbstractGizmo::transformAxis() const {
    return m_axis;
}

QVector<Mesh*>& AbstractGizmo::markers() {
    return m_markers;
}

void AbstractGizmo::bindTo(AbstractEntity * host) {
    if (m_host == host) return;
    if (m_host) unbind();
    if (host) {
        m_host = host;
        connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
#ifdef DEBUG_OUTPUT
        dout << this->objectName() << "is bound to" << host->objectName();
#endif
    }
}

void AbstractGizmo::unbind() {
    if (m_host) {
        disconnect(m_host, 0, this, 0);
        m_host = 0;
#ifdef DEBUG_OUTPUT
        dout << this->objectName() << "is unbound";
#endif
    }
}

void AbstractGizmo::setTransformAxis(void* marker) {
    for (int i = 0; i < m_markers.size(); i++)
        if (m_markers[i] == marker)
            m_axis = static_cast<AbstractGizmo::TransformAxis>(AbstractGizmo::X + i);
}

void AbstractGizmo::setTransformAxis(TransformAxis axis) {
    m_axis = axis;
}

void AbstractGizmo::hostDestroyed(QObject *) {
    unbind();
}
