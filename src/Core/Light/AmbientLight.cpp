#include <Core/Light/AmbientLight.h>

AmbientLight::AmbientLight(QVector3D color, QObject * parent): AbstractLight(color, parent) {}

void AmbientLight::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Ambient Light: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Enabled: " << m_enabled;
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
    qDebug().nospace() << tab(l + 1) << "Intensity: " << m_intensity;
}

void AmbientLight::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}
