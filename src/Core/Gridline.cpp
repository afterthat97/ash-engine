#include <Gridline.h>

Gridline::Gridline(QObject* parent): QObject(0) {
    int tmp_log_level = log_level;
    log_level = LOG_LEVEL_WARNING;

    m_marker = new Mesh(Mesh::Line, this);
    m_marker->setObjectName("Gridline Marker");
    m_marker->setMaterial(new Material);

    setObjectName("Untitled Gridline");
    reset();

    log_level = tmp_log_level;

    setParent(parent);
}

// Dump info

Gridline::Gridline(const Gridline & gridline): QObject(0) {
    int tmp_log_level = log_level;
    log_level = LOG_LEVEL_WARNING;

    m_marker = new Mesh(Mesh::Line, 0);
    m_marker->setMaterial(new Material);
    m_xRange = gridline.m_xRange;
    m_yRange = gridline.m_yRange;
    m_zRange = gridline.m_zRange;
    m_xStride = gridline.m_xStride;
    m_yStride = gridline.m_yStride;
    m_zStride = gridline.m_zStride;
    m_color = gridline.m_color;

    update();
    setObjectName(gridline.objectName());

    log_level = tmp_log_level;
}

Gridline::~Gridline() {
    int tmp_log_level = log_level;
    log_level = LOG_LEVEL_WARNING;

    delete m_marker;

    log_level = tmp_log_level;

    if (log_level >= LOG_LEVEL_INFO)
        dout << "Gridline" << this->objectName() << "is destroyed";
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

Mesh * Gridline::marker() {
    return m_marker;
}

// Public slots

void Gridline::reset() {
    m_xRange = { -20, 20 };
    m_yRange = { 0, 0 };
    m_zRange = { -20, 20 };
    m_xStride = m_yStride = m_zStride = 1;
    m_color = QVector3D(0.4f, 0.4f, 0.4f);
    update();
    if (log_level >= LOG_LEVEL_INFO)
        dout << this->objectName() << "is reset";
}

void Gridline::setXArguments(QVector3D xargs) {
    if (!isEqual(m_xRange.first, xargs[0]) || !isEqual(m_xRange.second, xargs[1]) || !isEqual(m_xStride, xargs[2])) {
        m_xRange = { xargs[0], xargs[1] };
        m_xStride = xargs[2];
        update();
        xArgumentsChanged(xargs);
    }
}

void Gridline::setYArguments(QVector3D yargs) {
    if (!isEqual(m_yRange.first, yargs[0]) || !isEqual(m_yRange.second, yargs[1]) || !isEqual(m_yStride, yargs[2])) {
        m_yRange = { yargs[0], yargs[1] };
        m_yStride = yargs[2];
        update();
        yArgumentsChanged(yargs);
    }
}

void Gridline::setZArguments(QVector3D zargs) {
    if (!isEqual(m_zRange.first, zargs[0]) || !isEqual(m_zRange.second, zargs[1]) || !isEqual(m_zStride, zargs[2])) {
        m_zRange = { zargs[0], zargs[1] };
        m_zStride = zargs[2];
        update();
        zArgumentsChanged(zargs);
    }
}

void Gridline::setColor(QVector3D color) {
    if (!isEqual(m_color, color)) {
        m_color = color;
        m_marker->material()->setColor(m_color);
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
    m_marker->setGeometry(vertices, indices);
    m_marker->material()->setColor(m_color);
}
