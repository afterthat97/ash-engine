#include <PointLight.h>
#include <ModelLoader.h>

PointLight::PointLight(QObject * parent): AbstractLight() {
    m_enableAttenuation = false;
    m_attenuationQuadratic = 0.0007f;
    m_attenuationLinear = 0.014f;
    m_attenuationConstant = 1.0f;

    initMarker();
    setParent(parent);
}

PointLight::PointLight(QVector3D color, QVector3D position, QObject * parent): AbstractLight(color) {
    m_position = position;
    m_enableAttenuation = false;
    m_attenuationQuadratic = 0.0007f;
    m_attenuationLinear = 0.014f;
    m_attenuationConstant = 1.0f;

    initMarker();
    setParent(parent);
}

PointLight::PointLight(const PointLight & light): AbstractLight(light) {
    m_position = light.m_position;
    m_enableAttenuation = light.m_enableAttenuation;
    m_attenuationQuadratic = light.m_attenuationQuadratic;
    m_attenuationLinear = light.m_attenuationLinear;
    m_attenuationConstant = light.m_attenuationConstant;

    initMarker();
}

PointLight::~PointLight() {
#ifdef DEBUG_OUTPUT
    dout << "Point Light" << objectName() << "is destroyed";
#endif
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

bool PointLight::visible() const {
    return m_marker->visible();
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

Mesh * PointLight::marker() const {
    return m_marker;
}

void PointLight::setColor(QVector3D color) {
    AbstractLight::setColor(color);
    m_marker->material()->setColor(color);
}

void PointLight::setVisible(bool visible) {
    m_marker->setVisible(visible);
}

void PointLight::setPosition(QVector3D position) {
    if (!isEqual(m_position, position)) {
        m_position = position;
        m_marker->setPosition(position);
        positionChanged(m_position);
    }
}

void PointLight::setEnabled(bool enabled) {
    AbstractLight::setEnabled(enabled);
    m_marker->setVisible(enabled);
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
    if (!isEqual(m_attenuationQuadratic, value)) {
        m_attenuationQuadratic = value;
        attenuationQuadraticChanged(m_attenuationQuadratic);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void PointLight::setAttenuationLinear(float value) {
    if (!isEqual(m_attenuationLinear, value)) {
        m_attenuationLinear = value;
        attenuationLinearChanged(m_attenuationLinear);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void PointLight::setAttenuationConstant(float value) {
    if (!isEqual(m_attenuationConstant, value)) {
        m_attenuationConstant = value;
        attenuationConstantChanged(m_attenuationConstant);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void PointLight::initMarker() {
    ModelLoader loader;

    m_marker = loader.loadMeshFromFile(":/resources/shapes/PointLight.obj");
    m_marker->setPosition(this->position());
    m_marker->setParent(this);

    connect(m_marker, SIGNAL(visibleChanged(bool)), this, SIGNAL(visibleChanged(bool)));
    connect(m_marker, SIGNAL(positionChanged(QVector3D)), this, SLOT(setPosition(QVector3D)));
}
