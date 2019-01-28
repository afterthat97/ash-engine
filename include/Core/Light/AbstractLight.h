#pragma once

#include <Core/Common.h>

class AbstractLight: public QObject {
    Q_OBJECT

public:
    AbstractLight();
    AbstractLight(QVector3D color);
    AbstractLight(const AbstractLight& light);
    ~AbstractLight();

    virtual void dumpObjectInfo(int level = 0) = 0;
    virtual void dumpObjectTree(int level = 0) = 0;

    QVector3D color();
    bool enabled();
    float intensity();

public slots:
    void setColor(QVector3D color);
    void setEnabled(bool enabled);
    void setIntensity(float intensity);

signals:
    void colorChanged(QVector3D color);
    void enabledChanged(bool enabled);
    void intensityChanged(float intensity);

protected:
    QVector3D m_color;
    bool m_enabled;
    float m_intensity;
};
