#include <OpenGL/OpenGLMesh.h>
#include <OpenGL/OpenGLMaterial.h>
#include <OpenGL/OpenGLConfig.h>
#include <OpenGL/OpenGLManager.h>

OpenGLMesh::OpenGLMesh(Mesh * mesh) {
    m_host = mesh;

    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(&m_host->vertices()[0], int(sizeof(Vertex) * m_host->vertices().size()));
    m_ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo->create();
    m_ebo->bind();
    m_ebo->allocate(&m_host->indices()[0], int(sizeof(uint32_t) * m_host->indices().size()));

    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    QVector<ShaderAttributeConfig> config = OpenGLConfig::getShaderAttributeConfig();
    for (int i = 0; i < config.size(); i++) {
        glFuncs->glEnableVertexAttribArray(config[i].indx);
        glFuncs->glVertexAttribPointer(config[i].indx, config[i].size, GL_FLOAT, GL_FALSE, config[i].stride, config[i].offset);
    }
    m_vao->release();

    connect(m_host, SIGNAL(geometryChanged(QVector<Vertex>, QVector<uint32_t>)),
            this, SLOT(geometryChanged(QVector<Vertex>, QVector<uint32_t>)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

OpenGLMesh::~OpenGLMesh() {
    delete m_vao;
    delete m_vbo;
    delete m_ebo;
}

Mesh * OpenGLMesh::host() const {
    return m_host;
}

void OpenGLMesh::render(QOpenGLShaderProgram* shader) {
    if (!m_host->visible()) return;
    if (m_host->material())
        OpenGLManager<Material, OpenGLMaterial>::currentManager()->getOpenGLObject(m_host->material())->bind(shader);
    shader->bind();
    shader->setUniformValue("modelMat", m_host->globalModelMatrix());
    m_vao->bind();
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    if (m_host->meshType() == Mesh::Triangle)
        glFuncs->glDrawElements(GL_TRIANGLES, (GLsizei) m_host->indices().size(), GL_UNSIGNED_INT, 0);
    else if (m_host->meshType() == Mesh::Line)
        glFuncs->glDrawElements(GL_LINES, (GLsizei) m_host->indices().size(), GL_UNSIGNED_INT, 0);
    else
        glFuncs->glDrawElements(GL_POINTS, (GLsizei) m_host->indices().size(), GL_UNSIGNED_INT, 0);
    m_vao->release();
    if (m_host->material())
        OpenGLManager<Material, OpenGLMaterial>::currentManager()->getOpenGLObject(m_host->material())->release();
}

void OpenGLMesh::geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices) {
    delete m_vao;
    delete m_vbo;
    delete m_ebo;
    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(&vertices[0], int(sizeof(Vertex) * vertices.size()));
    m_ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo->create();
    m_ebo->bind();
    m_ebo->allocate(&indices[0], int(sizeof(uint32_t) * indices.size()));
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    QVector<ShaderAttributeConfig> config = OpenGLConfig::getShaderAttributeConfig();
    for (int i = 0; i < config.size(); i++) {
        glFuncs->glEnableVertexAttribArray(config[i].indx);
        glFuncs->glVertexAttribPointer(config[i].indx, config[i].size, GL_FLOAT, GL_FALSE, config[i].stride, config[i].offset);
    }
    m_vao->release();
}

void OpenGLMesh::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<Mesh, OpenGLMesh>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
