#include <AbstractGizmo.h>

AbstractGizmo::AbstractGizmo(QObject* parent): AbstractEntity(0) {
    m_visible = false;
    m_axis = None;
    m_host = 0;
    setParent(parent);
}

AbstractGizmo::~AbstractGizmo() {}

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
#ifdef _DEBUG
        dout << this->objectName() << "is bound to" << host->objectName();
#endif
    }
}

void AbstractGizmo::unbind() {
    if (m_host) {
        disconnect(m_host, 0, this, 0);
        m_host = 0;
#ifdef _DEBUG
        dout << this->objectName() << "is unbound";
#endif
    }
}

void AbstractGizmo::setTransformAxis(TransformAxis axis) {
    m_axis = axis;
}

void AbstractGizmo::hostDestroyed(QObject *) {
    unbind();
}
