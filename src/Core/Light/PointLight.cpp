#include <Core/Light/PointLight.h>

PointLight::PointLight(QObject * parent): AbstractLight(parent) {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    m_position = QVector3D(0.0f, 0.0f, 0.0f);
    m_enableAttenuation = false;
    m_attenuationQuadratic = 0.0007f;
    m_attenuationLinear = 0.014f;
    m_attenuationConstant = 1.0f;
}

PointLight::PointLight(QVector3D color, QVector3D position, QObject * parent) : AbstractLight(color, parent) {
    m_position = position;
    m_enableAttenuation = false;
    m_attenuationQuadratic = 0.0007f;
    m_attenuationLinear = 0.014f;
    m_attenuationConstant = 1.0f;
}

PointLight::PointLight(const PointLight & light): AbstractLight(light) {
    m_position = light.m_position;
    m_enableAttenuation = light.m_enableAttenuation;
    m_attenuationQuadratic = light.m_attenuationQuadratic;
    m_attenuationLinear = light.m_attenuationLinear;
    m_attenuationConstant = light.m_attenuationConstant;
}

void PointLight::translate(QVector3D delta) {
    setPosition(m_position + delta);
}

void PointLight::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Point Light: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Enabled: " << m_enabled;
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
    qDebug().nospace() << tab(l + 1) << "Position: " << m_position;
    qDebug().nospace() << tab(l + 1) << "Intensity: " << m_intensity;
    qDebug().nospace() << tab(l + 1) << "Enable attenuation: " << m_enableAttenuation;
    if (m_enableAttenuation) {
        qDebug().nospace() << tab(l + 2) << "Quadratic value: " << m_attenuationQuadratic;
        qDebug().nospace() << tab(l + 2) << "Linear value:    " << m_attenuationLinear;
        qDebug().nospace() << tab(l + 2) << "Constant value:  " << m_attenuationConstant;
    }
}

void PointLight::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

QVector3D PointLight::position() const {
    return m_position;
}

bool PointLight::enableAttenuation() const {
    return m_enableAttenuation;
}

QVector3D PointLight::attenuationArguments() const {
    return QVector3D(m_attenuationQuadratic, m_attenuationLinear, m_attenuationConstant);
}

float PointLight::attenuationQuadratic() const {
    return m_attenuationQuadratic;
}

float PointLight::attenuationLinear() const {
    return m_attenuationLinear;
}

float PointLight::attenuationConstant() const {
    return m_attenuationConstant;
}

void PointLight::setPosition(QVector3D position) {
    if (!qFuzzyCompare(m_position, position)) {
        m_position = position;
        positionChanged(m_position);
    }
}

void PointLight::setEnableAttenuation(bool enabled) {
    if (m_enableAttenuation != enabled) {
        m_enableAttenuation = enabled;
        enableAttenuationChanged(m_enableAttenuation);
    }
}

void PointLight::setAttenuationArguments(QVector3D value) {
    setAttenuationQuadratic(value[0]);
    setAttenuationLinear(value[1]);
    setAttenuationConstant(value[2]);
}

void PointLight::setAttenuationQuadratic(float value) {
    if (!qFuzzyCompare(m_attenuationQuadratic, value)) {
        m_attenuationQuadratic = value;
        attenuationQuadraticChanged(m_attenuationQuadratic);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void PointLight::setAttenuationLinear(float value) {
    if (!qFuzzyCompare(m_attenuationLinear, value)) {
        m_attenuationLinear = value;
        attenuationLinearChanged(m_attenuationLinear);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void PointLight::setAttenuationConstant(float value) {
    if (!qFuzzyCompare(m_attenuationConstant, value)) {
        m_attenuationConstant = value;
        attenuationConstantChanged(m_attenuationConstant);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}
