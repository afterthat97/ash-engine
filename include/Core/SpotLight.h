#pragma once

#include <AbstractLight.h>

class SpotLight: public AbstractLight {
    Q_OBJECT

public:
    SpotLight(QObject* parent = 0);
    SpotLight(QVector3D color, QVector3D position, QVector3D direction, QObject* parent = 0);
    SpotLight(const SpotLight& light);
    ~SpotLight();

    void translate(QVector3D delta);

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;

    bool visible() const;
    QVector3D position() const;
    QVector3D direction() const;
    float innerCutOff() const;
    float outerCutOff() const;
    bool enableAttenuation() const;
    QVector3D attenuationArguments() const;
    float attenuationQuadratic() const;
    float attenuationLinear() const;
    float attenuationConstant() const;
    Mesh* marker() const override;

public slots:
    void setEnabled(bool enabled) override;
    void setColor(QVector3D color) override;
    void setVisible(bool visible);
    void setPosition(QVector3D position);
    void setDirection(QVector3D direction);
    void setInnerCutOff(float innerCutOff);
    void setOuterCutOff(float outerCutOff);
    void setEnableAttenuation(bool enabled);
    void setAttenuationArguments(QVector3D value);
    void setAttenuationQuadratic(float value);
    void setAttenuationLinear(float value);
    void setAttenuationConstant(float value);

signals:
    void visibleChanged(bool visible);
    void positionChanged(QVector3D position);
    void directionChanged(QVector3D direction);
    void innerCutOffChanged(float innerCutOff);
    void outerCutOffChanged(float outerCutOff);
    void enableAttenuationChanged(bool enabled);
    void attenuationArgumentsChanged(QVector3D value);
    void attenuationQuadraticChanged(float value);
    void attenuationLinearChanged(float value);
    void attenuationConstantChanged(float value);

protected:
    QVector3D m_position, m_direction;
    float m_innerCutOff, m_outerCutOff;
    bool m_enableAttenuation;
    float m_attenuationQuadratic, m_attenuationLinear, m_attenuationConstant;
    Mesh* m_marker;

    void initMarker();

private slots:
    void setDirectionFromRotation(QVector3D rotation);
};
