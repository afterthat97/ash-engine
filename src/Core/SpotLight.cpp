#include <SpotLight.h>
#include <ModelLoader.h>

SpotLight::SpotLight(QObject * parent): AbstractLight() {
    m_position = QVector3D(0.0f, 0.0f, 0.0f);
    m_direction = QVector3D(0.0f, -1.0f, 0.0f);
    m_innerCutOff = 30.0f;
    m_outerCutOff = 45.0f;
    m_enableAttenuation = false;
    m_attenuationQuadratic = 0.0007f;
    m_attenuationLinear = 0.014f;
    m_attenuationConstant = 1.0f;

    initMarker();
    setParent(parent);
}

SpotLight::SpotLight(QVector3D color, QVector3D position, QVector3D direction, QObject * parent)
: AbstractLight(color) {
    m_position = position;
    m_direction = direction;
    m_innerCutOff = 30.0f;
    m_outerCutOff = 45.0f;
    m_enableAttenuation = false;
    m_attenuationQuadratic = 0.0007f;
    m_attenuationLinear = 0.014f;
    m_attenuationConstant = 1.0f;

    initMarker();
    setParent(parent);
}

SpotLight::SpotLight(const SpotLight & light): AbstractLight(light) {
    m_position = light.m_position;
    m_direction = light.m_direction;
    m_innerCutOff = light.m_innerCutOff;
    m_outerCutOff = light.m_outerCutOff;
    m_enableAttenuation = light.m_enableAttenuation;
    m_attenuationQuadratic = light.m_attenuationQuadratic;
    m_attenuationLinear = light.m_attenuationLinear;
    m_attenuationConstant = light.m_attenuationConstant;

    initMarker();
}

SpotLight::~SpotLight() {
    delete m_marker;
#ifdef DEBUG_OUTPUT
    dout << "Spot Light" << objectName() << "is destroyed";
#endif
}

void SpotLight::translate(QVector3D delta) {
    setPosition(m_position + delta);
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

QVector3D SpotLight::position() const {
    return m_position;
}

QVector3D SpotLight::direction() const {
    return m_direction;
}

float SpotLight::innerCutOff() const {
    return m_innerCutOff;
}

float SpotLight::outerCutOff() const {
    return m_outerCutOff;
}

bool SpotLight::enableAttenuation() const {
    return m_enableAttenuation;
}

QVector3D SpotLight::attenuationArguments() const {
    return QVector3D(m_attenuationQuadratic, m_attenuationLinear, m_attenuationConstant);
}

float SpotLight::attenuationQuadratic() const {
    return m_attenuationQuadratic;
}

float SpotLight::attenuationLinear() const {
    return m_attenuationLinear;
}

float SpotLight::attenuationConstant() const {
    return m_attenuationConstant;
}

Mesh * SpotLight::marker() const {
    return m_marker;
}

void SpotLight::setColor(QVector3D color) {
    AbstractLight::setColor(color);
    m_marker->material()->setColor(color);
}

void SpotLight::setEnabled(bool enabled) {
    AbstractLight::setEnabled(enabled);
    m_marker->setVisible(enabled);
}

void SpotLight::setPosition(QVector3D position) {
    if (!isEqual(m_position, position)) {
        m_position = position;
        m_marker->setPosition(m_position);
        positionChanged(m_position);
    }
}

void SpotLight::setDirection(QVector3D direction) {
    if (!isEqual(m_direction, direction)) {
        m_direction = direction;
        m_marker->setRotation(QQuaternion::rotationTo(QVector3D(0, -1, 0), m_direction));
        directionChanged(m_direction);
    }
}

void SpotLight::setInnerCutOff(float innerCutOff) {
    if (!isEqual(m_innerCutOff, innerCutOff)) {
        m_innerCutOff = innerCutOff;
        innerCutOffChanged(m_innerCutOff);
    }
}

void SpotLight::setOuterCutOff(float outerCutOff) {
    if (!isEqual(m_outerCutOff, outerCutOff)) {
        m_outerCutOff = outerCutOff;
        outerCutOffChanged(m_outerCutOff);
    }
}

void SpotLight::setEnableAttenuation(bool enabled) {
    if (m_enableAttenuation != enabled) {
        m_enableAttenuation = enabled;
        enableAttenuationChanged(m_enableAttenuation);
    }
}

void SpotLight::setAttenuationArguments(QVector3D value) {
    setAttenuationQuadratic(value[0]);
    setAttenuationLinear(value[1]);
    setAttenuationConstant(value[2]);
}

void SpotLight::setAttenuationQuadratic(float value) {
    if (!isEqual(m_attenuationQuadratic, value)) {
        m_attenuationQuadratic = value;
        attenuationQuadraticChanged(m_attenuationQuadratic);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void SpotLight::setAttenuationLinear(float value) {
    if (!isEqual(m_attenuationLinear, value)) {
        m_attenuationLinear = value;
        attenuationLinearChanged(m_attenuationLinear);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void SpotLight::setAttenuationConstant(float value) {
    if (!isEqual(m_attenuationConstant, value)) {
        m_attenuationConstant = value;
        attenuationConstantChanged(m_attenuationConstant);
        attenuationArgumentsChanged(this->attenuationArguments());
    }
}

void SpotLight::setDirectionFromRotation(QVector3D rotation) {
    setDirection(QQuaternion::fromEulerAngles(rotation) * QVector3D(0, -1, 0));
}

void SpotLight::initMarker() {
    ModelLoader loader;
    m_marker = loader.loadMeshFromFile(":/resources/shapes/SpotLight.obj");
    m_marker->setPosition(this->position());
    m_marker->setRotation(QQuaternion::rotationTo(QVector3D(0, -1, 0), this->direction()));
    m_marker->setParent(this);

    connect(m_marker, SIGNAL(positionChanged(QVector3D)), this, SLOT(setPosition(QVector3D)));
    connect(m_marker, SIGNAL(rotationChanged(QVector3D)), this, SLOT(setDirectionFromRotation(QVector3D)));
}
