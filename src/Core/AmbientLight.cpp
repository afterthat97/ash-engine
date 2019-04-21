#include <AmbientLight.h>

AmbientLight::AmbientLight(QObject * parent) : AbstractLight() {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    setObjectName("Untitled Ambient Light");
    setParent(parent);
}

AmbientLight::AmbientLight(QVector3D color, QObject * parent) : AbstractLight(color) {
    setObjectName("Untitled Ambient Light");
    setParent(parent);
}

AmbientLight::AmbientLight(const AmbientLight & light) : AbstractLight(light) {
    setObjectName(light.objectName());
}

AmbientLight::~AmbientLight() {
    if (log_level >= LOG_LEVEL_INFO)
        dout << "Ambient light" << this->objectName() << "is destroyed";
}

void AmbientLight::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Ambient Light: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Enabled: " << m_enabled;
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
    qDebug().nospace() << tab(l + 1) << "Intensity: " << m_intensity;
}

void AmbientLight::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}
