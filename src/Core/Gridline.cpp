#include <Gridline.h>

Gridline::Gridline(QObject* parent): QObject(0) {
    m_gridlineMesh = new Mesh(Mesh::Line, this);
    m_gridlineMesh->setMaterial(new Material);
    reset();
    setParent(parent);
}

// Dump info

Gridline::Gridline(const Gridline & gridline): QObject(0) {
    setObjectName(gridline.objectName());
    m_gridlineMesh = new Mesh(Mesh::Line, 0);
    m_gridlineMesh->setMaterial(new Material);
    m_xRange = gridline.m_xRange;
    m_yRange = gridline.m_yRange;
    m_zRange = gridline.m_zRange;
    m_xStride = gridline.m_xStride;
    m_yStride = gridline.m_yStride;
    m_zStride = gridline.m_zStride;
    m_color = gridline.m_color;
    update();
}

Gridline::~Gridline() {
    delete m_gridlineMesh;
#ifdef _DEBUG
    qDebug() << "Gridline" << this->objectName() << "is destroyed";
#endif
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

Mesh * Gridline::gridlineMesh() {
    return m_gridlineMesh;
}

// Public slots

void Gridline::reset() {
    m_xRange = { -20, 20 };
    m_yRange = { 0, 0 };
    m_zRange = { -20, 20 };
    m_xStride = m_yStride = m_zStride = 1;
    m_color = QVector3D(0.4f, 0.4f, 0.4f);
    update();
}

void Gridline::setXArguments(QVector3D xargs) {
    if (!qFuzzyCompare(m_xRange.first, xargs[0]) || !qFuzzyCompare(m_xRange.second, xargs[1]) || !qFuzzyCompare(m_xStride, xargs[2])) {
        m_xRange = { xargs[0], xargs[1] };
        m_xStride = xargs[2];
        update();
        xArgumentsChanged(xargs);
    }
}

void Gridline::setYArguments(QVector3D yargs) {
    if (!qFuzzyCompare(m_yRange.first, yargs[0]) || !qFuzzyCompare(m_yRange.second, yargs[1]) || !qFuzzyCompare(m_yStride, yargs[2])) {
        m_yRange = { yargs[0], yargs[1] };
        m_yStride = yargs[2];
        update();
        yArgumentsChanged(yargs);
    }
}

void Gridline::setZArguments(QVector3D zargs) {
    if (!qFuzzyCompare(m_zRange.first, zargs[0]) || !qFuzzyCompare(m_zRange.second, zargs[1]) || !qFuzzyCompare(m_zStride, zargs[2])) {
        m_zRange = { zargs[0], zargs[1] };
        m_zStride = zargs[2];
        update();
        zArgumentsChanged(zargs);
    }
}

void Gridline::setColor(QVector3D color) {
    if (!qFuzzyCompare(m_color, color)) {
        m_color = color;
        m_gridlineMesh->material()->setColor(m_color);
        colorChanged(m_color);
    }
}

void Gridline::update() {
    QVector<Vertex> vertices;
    QVector<uint32_t> indices;
    for (float yValue = m_yRange.first; yValue < m_yRange.second + 0.01f; yValue += m_yStride) {
        for (float xValue = m_xRange.first; xValue < m_xRange.second + 0.01f; xValue += m_xStride) {
            vertices.push_back(Vertex(QVector3D(xValue, yValue, m_zRange.first)));
            vertices.push_back(Vertex(QVector3D(xValue, yValue, m_zRange.second)));
            indices.push_back((uint32_t) vertices.size() - 2);
            indices.push_back((uint32_t) vertices.size() - 1);
        }
        for (float zValue = m_zRange.first; zValue < m_zRange.second + 0.01f; zValue += m_zStride) {
            vertices.push_back(Vertex(QVector3D(m_xRange.first, yValue, zValue)));
            vertices.push_back(Vertex(QVector3D(m_xRange.second, yValue, zValue)));
            indices.push_back((uint32_t) vertices.size() - 2);
            indices.push_back((uint32_t) vertices.size() - 1);
        }
    }
    m_gridlineMesh->setGeometry(vertices, indices);
    m_gridlineMesh->material()->setColor(m_color);
}
