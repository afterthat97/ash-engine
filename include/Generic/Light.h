#pragma once

#include <Generic/Object.h>
#include <Generic/Model.h>
#include <Generic/Allocator.h>

class Light: public Object {
public:
    Light(QVector3D position = QVector3D());
    ~Light();

    void setColor(QVector3D newColor);
    void setPosition(QVector3D newPosition);
    void setEnableAttenuation(bool enabled);
    void setAttenuationQuadratic(float value);
    void setAttenuationLinear(float value);
    void setAttenuationConstant(float value);
    
    QVector3D getColor();
    QVector3D getPosition();
    bool isAttenuationEnabled();
    float getAttenuationQuadratic();
    float getAttenuationLinear();
    float getAttenuationConstant();

    Model* getLightBulbModel();

private:
    Model * lightBulbModel;
    QVector3D color, position;
    bool enableAttenuation;
    float attenuationQuadratic, attenuationLinear, attenuationConstant;

    void initID() override;
};
