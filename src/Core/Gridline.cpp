#include <Core/Gridline.h>
#include <Core/Model.h>

Gridline::Gridline(QObject* parent): QObject(parent) {
    reset();
}

// Dump info

Gridline::Gridline(const Gridline & gridline) {
    m_xRange = gridline.m_xRange;
    m_yRange = gridline.m_yRange;
    m_zRange = gridline.m_zRange;
    m_xStride = gridline.m_xStride;
    m_yStride = gridline.m_yStride;
    m_zStride = gridline.m_zStride;
    m_color = gridline.m_color;
}

void Gridline::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Gridline: " << objectName();
    qDebug().nospace() << tab(l + 1) << "X Range: " << m_xRange;
    qDebug().nospace() << tab(l + 1) << "Y Range: " << m_yRange;
    qDebug().nospace() << tab(l + 1) << "Z Range: " << m_zRange;
    qDebug().nospace() << tab(l + 1) << "X Stride: " << m_xStride;
    qDebug().nospace() << tab(l + 1) << "Y Stride: " << m_yStride;
    qDebug().nospace() << tab(l + 1) << "Z Stride: " << m_zStride;
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
}

void Gridline::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

// Get properties

QPair<float, float> Gridline::xRange() const {
    return m_xRange;
}

QPair<float, float> Gridline::yRange() const {
    return m_yRange;
}

QPair<float, float> Gridline::zRange() const {
    return m_zRange;
}

float Gridline::xStride() const {
    return m_xStride;
}

float Gridline::yStride() const {
    return m_yStride;
}

float Gridline::zStride() const {
    return m_zStride;
}

QVector3D Gridline::color() const {
    return m_color;
}

// Public slots

void Gridline::reset() {
    setXRange(-100.0f, 100.0f);
    setYRange(0.0f, 0.0f);
    setZRange(-100.0f, 100.0f);
    setStride(4.0f, 4.0f, 4.0f);
    setColor(QVector3D(0.4f, 0.4f, 0.4f));
}

void Gridline::setXRange(float start, float end) {
    if (!qFuzzyCompare(m_xRange.first, start) || !qFuzzyCompare(m_xRange.second, end)) {
        m_xRange = { start, end };
        xRangeChanged(m_xRange.first, m_xRange.second);
        xArgumentsChanged(QVector3D(m_xRange.first, m_xRange.second, m_xStride));
    }
}

void Gridline::setYRange(float start, float end) {
    if (!qFuzzyCompare(m_yRange.first, start) || !qFuzzyCompare(m_yRange.second, end)) {
        m_yRange = { start, end };
        yRangeChanged(m_yRange.first, m_yRange.second);
        yArgumentsChanged(QVector3D(m_yRange.first, m_yRange.second, m_yStride));
    }
}

void Gridline::setZRange(float start, float end) {
    if (!qFuzzyCompare(m_zRange.first, start) || !qFuzzyCompare(m_zRange.second, end)) {
        m_zRange = { start, end };
        zRangeChanged(m_zRange.first, m_zRange.second);
        zArgumentsChanged(QVector3D(m_zRange.first, m_zRange.second, m_zStride));
    }
}

void Gridline::setXStride(float xStride) {
    if (!qFuzzyCompare(m_xStride, xStride)) {
        m_xStride = xStride;
        strideChanged(m_xStride, m_yStride, m_zStride);
        xArgumentsChanged(QVector3D(m_xRange.first, m_xRange.second, m_xStride));
    }
}

void Gridline::setYStride(float yStride) {
    if (!qFuzzyCompare(m_yStride, yStride)) {
        m_yStride = yStride;
        strideChanged(m_xStride, m_yStride, m_zStride);
        yArgumentsChanged(QVector3D(m_yRange.first, m_yRange.second, m_yStride));
    }
}

void Gridline::setZStride(float zStride) {
    if (!qFuzzyCompare(m_zStride, zStride)) {
        m_zStride = zStride;
        strideChanged(m_xStride, m_yStride, m_zStride);
        zArgumentsChanged(QVector3D(m_zRange.first, m_zRange.second, m_zStride));
    }
}

void Gridline::setStride(float x, float y, float z) {
    if (!qFuzzyCompare(m_xStride, x) || !qFuzzyCompare(m_yStride, y) || !qFuzzyCompare(m_zStride, z)) {
        m_xStride = x; m_yStride = y; m_zStride = z;
        strideChanged(m_xStride, m_yStride, m_zStride);
        xArgumentsChanged(QVector3D(m_xRange.first, m_xRange.second, m_xStride));
        yArgumentsChanged(QVector3D(m_yRange.first, m_yRange.second, m_yStride));
        zArgumentsChanged(QVector3D(m_zRange.first, m_zRange.second, m_zStride));
    }
}

void Gridline::setXArguments(QVector3D xargs) {
    setXRange(xargs[0], xargs[1]);
    setXStride(xargs[2]);
}

void Gridline::setYArguments(QVector3D yargs) {
    setYRange(yargs[0], yargs[1]);
    setYStride(yargs[2]);
}

void Gridline::setZArguments(QVector3D zargs) {
    setZRange(zargs[0], zargs[1]);
    setZStride(zargs[2]);
}

void Gridline::setColor(QVector3D color) {
    if (!qFuzzyCompare(m_color, color)) {
        m_color = color;
        colorChanged(m_color);
    }
}