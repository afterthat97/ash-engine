#pragma once

#include <Core/Common.h>

class AbstractLight: public QObject {
    Q_OBJECT

public:
    AbstractLight(QVector3D color = QVector3D(1, 1, 1), QObject* parent = 0);
    // TODO: Copy constructor

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
    float m_intensity;
    bool m_enabled;
};
