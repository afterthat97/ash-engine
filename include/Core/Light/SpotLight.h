#pragma once

#include <Core/Light/PointLight.h>

class SpotLight: public PointLight {
    Q_OBJECT

public:
    SpotLight(QVector3D color = QVector3D(1, 1, 1),
              QVector3D position = QVector3D(0, 0, 0),
              QVector3D direction = QVector3D(0, -1, 0),
              QObject* parent = 0);
    // TODO: Copy constructor

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;

    float innerCutOff();
    float outerCutOff();
    QVector3D direction();

public slots:
    void setInnerCutOff(float innerCutOff);
    void setOuterCutOff(float outerCutOff);
    void setDirection(QVector3D direction);

signals:
    void innerCutOffChanged(float innerCutOff);
    void outerCutOffChanged(float outerCutOff);
    void directionChanged(QVector3D direction);

protected:
    float m_innerCutOff, m_outerCutOff;
    QVector3D m_direction;
};
