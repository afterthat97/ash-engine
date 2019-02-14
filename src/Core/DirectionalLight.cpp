#include <DirectionalLight.h>

DirectionalLight::DirectionalLight(QObject * parent) : AbstractLight() {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    m_direction = QVector3D(-1.0f, -1.0f, -1.0f);
    setParent(parent);
}

DirectionalLight::DirectionalLight(QVector3D color, QVector3D direction, QObject * parent): AbstractLight(color) {
    m_direction = direction;
    setParent(parent);
}

DirectionalLight::DirectionalLight(const DirectionalLight & light): AbstractLight(light) {
    m_direction = light.m_direction;
}

DirectionalLight::~DirectionalLight() {
#ifdef DEBUG_OUTPUT
    dout << "Directional Light" << objectName() << "is destroyed";
#endif
}

void DirectionalLight::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Directional Light: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Enabled: " << m_enabled;
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
    qDebug().nospace() << tab(l + 1) << "Direction: " << m_direction;
    qDebug().nospace() << tab(l + 1) << "Intensity: " << m_intensity;
}

void DirectionalLight::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

QVector3D DirectionalLight::direction() {
    return m_direction;
}

void DirectionalLight::setDirection(QVector3D direction) {
    if (!isEqual(m_direction, direction)) {
        m_direction = direction;
        directionChanged(m_direction);
    }
}
