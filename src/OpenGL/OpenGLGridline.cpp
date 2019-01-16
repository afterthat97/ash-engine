#include <OpenGL/OpenGLGridline.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLMesh.h>

OpenGLGridline::OpenGLGridline(Gridline * gridline) {
    m_host = gridline;
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
    connect(m_host, SIGNAL(xArgumentsChanged(QVector3D)), this, SLOT(xArgumentsChanged(QVector3D)));
    connect(m_host, SIGNAL(yArgumentsChanged(QVector3D)), this, SLOT(yArgumentsChanged(QVector3D)));
    connect(m_host, SIGNAL(zArgumentsChanged(QVector3D)), this, SLOT(zArgumentsChanged(QVector3D)));
    m_gridlineMesh = new Mesh(Mesh::Line, 0);
    update();
}

OpenGLGridline::~OpenGLGridline() {
    delete m_gridlineMesh;
}

Gridline * OpenGLGridline::host() const {
    return m_host;
}

void OpenGLGridline::render(QOpenGLShaderProgram * shader) {
    if (m_needUpdate) update();
    shader->bind();
    shader->setUniformValue("color", m_host->color());
    OpenGLManager<Mesh, OpenGLMesh>::currentManager()->getOpenGLObject(m_gridlineMesh)->render(shader);
}

void OpenGLGridline::update() {
    QVector<Vertex> vertices;
    QVector<uint32_t> indices;
    for (float yValue = m_host->yRange().first; yValue < m_host->yRange().second + 0.01f; yValue += m_host->yStride()) {
        for (float xValue = m_host->xRange().first; xValue < m_host->xRange().second + 0.01f; xValue += m_host->xStride()) {
            vertices.push_back(Vertex(QVector3D(xValue, yValue, m_host->zRange().first)));
            vertices.push_back(Vertex(QVector3D(xValue, yValue, m_host->zRange().second)));
            indices.push_back((uint32_t) vertices.size() - 2);
            indices.push_back((uint32_t) vertices.size() - 1);
        }
        for (float zValue = m_host->zRange().first; zValue < m_host->zRange().second + 0.01f; zValue += m_host->zStride()) {
            vertices.push_back(Vertex(QVector3D(m_host->xRange().first, yValue, zValue)));
            vertices.push_back(Vertex(QVector3D(m_host->xRange().second, yValue, zValue)));
            indices.push_back((uint32_t) vertices.size() - 2);
            indices.push_back((uint32_t) vertices.size() - 1);
        }
    }
    m_gridlineMesh->setGeometry(vertices, indices);
    m_needUpdate = false;
}

void OpenGLGridline::xArgumentsChanged(QVector3D) {
    m_needUpdate = true;
}

void OpenGLGridline::yArgumentsChanged(QVector3D) {
    m_needUpdate = true;
}

void OpenGLGridline::zArgumentsChanged(QVector3D) {
    m_needUpdate = true;
}

void OpenGLGridline::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<Gridline, OpenGLGridline>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
