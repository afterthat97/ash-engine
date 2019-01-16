#include <Core/Light/SpotLight.h>

SpotLight::SpotLight(QVector3D color, QVector3D position, QVector3D direction, QObject * parent)
: PointLight(color, position, parent) {
    m_innerCutOff = 30.0f;
    m_outerCutOff = 45.0f;
    m_direction = direction;
}

void SpotLight::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Spot Light: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Enabled: " << m_enabled;
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
    qDebug().nospace() << tab(l + 1) << "Position: " << m_position;
    qDebug().nospace() << tab(l + 1) << "Direction: " << m_direction;
    qDebug().nospace() << tab(l + 1) << "Intensity: " << m_intensity;
    qDebug().nospace() << tab(l + 1) << "Inner Cut Off Angle: " << m_innerCutOff;
    qDebug().nospace() << tab(l + 1) << "Outer Cut Off Angle: " << m_outerCutOff;
    qDebug().nospace() << tab(l + 1) << "Enable attenuation: " << m_enableAttenuation;
    if (m_enableAttenuation) {
        qDebug().nospace() << tab(l + 2) << "Quadratic value: " << m_attenuationQuadratic;
        qDebug().nospace() << tab(l + 2) << "Linear value:    " << m_attenuationLinear;
        qDebug().nospace() << tab(l + 2) << "Constant value:  " << m_attenuationConstant;
    }
}

void SpotLight::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

float SpotLight::innerCutOff() {
    return m_innerCutOff;
}

float SpotLight::outerCutOff() {
    return m_outerCutOff;
}

QVector3D SpotLight::direction() {
    return m_direction;
}

void SpotLight::setInnerCutOff(float innerCutOff) {
    if (!qFuzzyCompare(m_innerCutOff, innerCutOff)) {
        m_innerCutOff = innerCutOff;
        innerCutOffChanged(m_innerCutOff);
    }
}

void SpotLight::setOuterCutOff(float outerCutOff) {
    if (!qFuzzyCompare(m_outerCutOff, outerCutOff)) {
        m_outerCutOff = outerCutOff;
        outerCutOffChanged(m_outerCutOff);
    }
}

void SpotLight::setDirection(QVector3D direction) {
    if (!qFuzzyCompare(m_direction, direction)) {
        m_direction = direction;
        directionChanged(m_direction);
    }
}
