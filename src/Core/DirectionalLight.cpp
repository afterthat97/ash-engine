#include <DirectionalLight.h>

DirectionalLight::DirectionalLight(QObject * parent) : AbstractLight() {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    m_direction = QVector3D(-1.0f, -1.0f, -1.0f);
    setObjectName("Untitled Directional Light");
    setParent(parent);
}

DirectionalLight::DirectionalLight(QVector3D color, QVector3D direction, QObject * parent): AbstractLight(color) {
    m_direction = direction;
    setObjectName("Untitled Directional Light");
    setParent(parent);
}

DirectionalLight::DirectionalLight(const DirectionalLight & light): AbstractLight(light) {
    m_direction = light.m_direction;
    setObjectName(light.objectName());
}

DirectionalLight::~DirectionalLight() {
    if (log_level >= LOG_LEVEL_INFO)
        dout << "Directional light" << this->objectName() << "is destroyed";
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
        if (log_level >= LOG_LEVEL_INFO)
            dout << "The direction of" << this->objectName() << "is set to" << direction;
        directionChanged(m_direction);
    }
}
