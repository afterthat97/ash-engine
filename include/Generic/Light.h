#pragma once

#include <Generic/Model.h>
#include <Generic/Allocator.h>

class Light: public QObject {
    Q_OBJECT

public:
    Light(QVector3D position = QVector3D(), QObject * parent = 0);
    ~Light();

    QVector3D getColor();
    QVector3D getPosition();
    bool isAttenuationEnabled();
    float getAttenuationQuadratic();
    float getAttenuationLinear();
    float getAttenuationConstant();
    Model* getLightBulbModel();

    void translate(QVector3D delta);

public slots:
    void setColor(QVector3D newColor);
    void setPosition(QVector3D newPosition);
    void setEnableAttenuation(bool enabled);
    void setAttenuationValue(QVector3D value);
    void setAttenuationQuadratic(float value);
    void setAttenuationLinear(float value);
    void setAttenuationConstant(float value);

signals:
    void positionChanged(QVector3D position);

private:
    uint32_t id;
    Model * lightBulbModel;
    QVector3D color, position;
    bool enableAttenuation;
    float attenuationQuadratic, attenuationLinear, attenuationConstant;
};
