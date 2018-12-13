#include <Generic/Light.h>
#include <IO/Loader.h>

Light::Light(QVector3D _position): Object() {
    lightBulbModel = Loader::loadFromFile(":/resources/shapes/light.fbx");

    initID();
    initName();
    setColor(QVector3D(1.0f, 1.0f, 1.0f));
    setPosition(_position);
    setEnableAttenuation(true);
    setAttenuationQuadratic(0.000007f);
    setAttenuationLinear(0.0014f);
    setAttenuationConstant(1.0f);
}

Light::~Light() {
    delete lightBulbModel;
}

void Light::setColor(QVector3D newColor) {
    color = newColor;
    lightBulbModel->getMeshes()[1]->getMaterial()->setDiffuseColor(color);
}

void Light::setPosition(QVector3D newPosition) {
    position = newPosition;
    lightBulbModel->getMeshes()[0]->setPosition(position);
    lightBulbModel->getMeshes()[1]->setPosition(position);
}

void Light::setEnableAttenuation(bool enabled) {
    enableAttenuation = enabled;
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

void Light::initName() {
    name = "Light " + QString(to_string(id).c_str());
}

void Light::initID() {
    id = Allocator::allocateLightID();
}
