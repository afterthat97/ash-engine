#include <Generic/Light.h>
#include <IO/Loader.h>

Light::Light(QVector3D _position, QObject * parent): QObject(parent) {
    lightBulbModel = Loader::loadFromFile(":/resources/shapes/light.fbx");

    id = Allocator::allocateLightID();
    setObjectName("Light " + QString::number(id));
    setColor(QVector3D(1.0f, 1.0f, 1.0f));
    setPosition(_position);
    setEnableAttenuation(true);
    setAttenuationQuadratic(0.0007f);
    setAttenuationLinear(0.014f);
    setAttenuationConstant(1.0f);
}

Light::~Light() {
    delete lightBulbModel;
}

// Get properties

QVector3D Light::getColor() {
    return color;
}

QVector3D Light::getPosition() {
    return position;
}

bool Light::isAttenuationEnabled() {
    return enableAttenuation;
}

float Light::getAttenuationQuadratic() {
    return attenuationQuadratic;
}

float Light::getAttenuationLinear() {
    return attenuationLinear;
}

float Light::getAttenuationConstant() {
    return attenuationConstant;
}

Model * Light::getLightBulbModel() {
    return lightBulbModel;
}

// Transform functions

void Light::translate(QVector3D delta) {
    setPosition(position + delta);
    positionChanged(position);
}

// Public slots

void Light::setColor(QVector3D newColor) {
    color = newColor;
    lightBulbModel->getMeshes()[1]->getMaterial()->setDiffuseColor(color);
}

void Light::setPosition(QVector3D newPosition) {
    position = newPosition;
    lightBulbModel->setPosition(position);
}

void Light::setEnableAttenuation(bool enabled) {
    enableAttenuation = enabled;
}

void Light::setAttenuationValue(QVector3D value) {
    setAttenuationQuadratic(value[0]);
    setAttenuationLinear(value[1]);
    setAttenuationConstant(value[2]);
}

void Light::setAttenuationQuadratic(float value) {
    attenuationQuadratic = value;
}

void Light::setAttenuationLinear(float value) {
    attenuationLinear = value;
}

void Light::setAttenuationConstant(float value) {
    attenuationConstant = value;
}
