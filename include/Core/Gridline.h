#pragma once

#include <Core/Common.h>

class Gridline: public QObject {
    Q_OBJECT

public:
    Gridline(QObject* parent = 0);
    // TODO: Copy constructor

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    QPair<float, float> xRange() const;
    QPair<float, float> yRange() const;
    QPair<float, float> zRange() const;
    float xStride() const;
    float yStride() const;
    float zStride() const;
    QVector3D color() const;

public slots:
    void reset();
    void setXRange(float start, float end);
    void setYRange(float start, float end);
    void setZRange(float start, float end);
    void setXStride(float xStride);
    void setYStride(float yStride);
    void setZStride(float zStride);
    void setStride(float xStride, float yStride, float zStride);
    void setXArguments(QVector3D xargs);
    void setYArguments(QVector3D yargs);
    void setZArguments(QVector3D zargs);
    void setColor(QVector3D color);

signals:
    void xRangeChanged(float start, float end);
    void yRangeChanged(float start, float end);
    void zRangeChanged(float start, float end);
    void xStrideChanged(float xStride);
    void yStrideChanged(float yStride);
    void zStrideChanged(float zStride);
    void strideChanged(float xStride, float yStride, float zStride);
    void xArgumentsChanged(QVector3D xargs);
    void yArgumentsChanged(QVector3D yargs);
    void zArgumentsChanged(QVector3D zargs);
    void colorChanged(QVector3D color);

private:
    QPair<float, float> m_xRange, m_yRange, m_zRange;
    float m_xStride, m_yStride, m_zStride;
    QVector3D m_color;
};
