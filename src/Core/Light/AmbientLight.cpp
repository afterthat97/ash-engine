#include <Core/Light/AmbientLight.h>

AmbientLight::AmbientLight(QObject * parent) : AbstractLight() {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    setParent(parent);
}

AmbientLight::AmbientLight(QVector3D color, QObject * parent) : AbstractLight(color) {
    setParent(parent);
}

AmbientLight::AmbientLight(const AmbientLight & light) : AbstractLight(light) {}

AmbientLight::~AmbientLight() {
#ifdef _DEBUG
    qDebug() << "Ambient Light" << this->objectName() << "is destroyed";
#endif
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
