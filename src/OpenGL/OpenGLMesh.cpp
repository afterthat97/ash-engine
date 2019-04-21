#include <OpenGLMesh.h>
#include <OpenGLMaterial.h>

struct ShaderModelInfo {
    float modelMat[16];   // 64          // 0
    float normalMat[16];  // 64          // 64
    int sizeFixed;        // 4           // 128
    int selected;         // 4           // 132
    int highlighted;      // 4           // 136
    uint pickingID;        // 4           // 140
};

static ShaderModelInfo shaderModelInfo;

OpenGLUniformBufferObject *OpenGLMesh::m_modelInfo = 0;

OpenGLMesh::OpenGLMesh(Mesh * mesh, QObject* parent): QObject(0) {
    m_host = mesh;
    m_sizeFixed = false;
    m_pickingID = 0;
    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
    if (m_host->material())
        m_openGLMaterial = new OpenGLMaterial(m_host->material());
    else
        m_openGLMaterial = 0;

    connect(m_host, SIGNAL(materialChanged(Material*)), this, SLOT(materialChanged(Material*)));
    connect(m_host, SIGNAL(geometryChanged(QVector<Vertex>, QVector<uint32_t>)), this, SLOT(geometryChanged(QVector<Vertex>, QVector<uint32_t>)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

    setParent(parent);
}

OpenGLMesh::~OpenGLMesh() {
    this->destroy();
}

Mesh * OpenGLMesh::host() const {
    return m_host;
}

void OpenGLMesh::create() {
    this->destroy();

    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo->create();
    m_vbo->bind();
    if (m_host->vertices().size())
        m_vbo->allocate(&m_host->vertices()[0], int(sizeof(Vertex) * m_host->vertices().size()));
    m_ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo->create();
    m_ebo->bind();
    if (m_host->indices().size())
        m_ebo->allocate(&m_host->indices()[0], int(sizeof(uint32_t) * m_host->indices().size()));

    glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glEnableVertexAttribArray(1);
    glFuncs->glEnableVertexAttribArray(2);
    glFuncs->glEnableVertexAttribArray(3);
    glFuncs->glEnableVertexAttribArray(4);
    glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glFuncs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glFuncs->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tangent));
    glFuncs->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, bitangent));
    glFuncs->glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));

    m_vao->release();
}

void OpenGLMesh::commit() {
    QMatrix4x4 modelMat = m_host->globalModelMatrix();

    memcpy(shaderModelInfo.modelMat, modelMat.constData(), 64);
    memcpy(shaderModelInfo.normalMat, QMatrix4x4(modelMat.normalMatrix()).constData(), 64);
    shaderModelInfo.sizeFixed = this->m_sizeFixed;
    shaderModelInfo.selected = m_host->selected();
    shaderModelInfo.highlighted = m_host->highlighted();
    shaderModelInfo.pickingID = this->m_pickingID;

    if (m_modelInfo == 0) {
        m_modelInfo = new OpenGLUniformBufferObject;
        m_modelInfo->create();
        m_modelInfo->bind();
        m_modelInfo->allocate(MODEL_INFO_BINDING_POINT, NULL, sizeof(ShaderModelInfo));
        m_modelInfo->release();
    }
    m_modelInfo->bind();
    m_modelInfo->write(0, &shaderModelInfo, sizeof(ShaderModelInfo));
    m_modelInfo->release();
}

void OpenGLMesh::render(bool pickingPass) {
    if (!m_host->visible()) return;
    if (m_vao == 0 || m_vbo == 0 || m_ebo == 0) create();

    commit();

    if (!pickingPass && m_host->wireFrameMode())
        glFuncs->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (m_openGLMaterial)
        m_openGLMaterial->bind();

    m_vao->bind();

    if (m_host->meshType() == Mesh::Triangle)
        glFuncs->glDrawElements(GL_TRIANGLES, (GLsizei) m_host->indices().size(), GL_UNSIGNED_INT, 0);
    else if (m_host->meshType() == Mesh::Line)
        glFuncs->glDrawElements(GL_LINES, (GLsizei) m_host->indices().size(), GL_UNSIGNED_INT, 0);
    else
        glFuncs->glDrawElements(GL_POINTS, (GLsizei) m_host->indices().size(), GL_UNSIGNED_INT, 0);

    m_vao->release();

    if (!pickingPass && m_host->wireFrameMode())
        glFuncs->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if (m_openGLMaterial)
        m_openGLMaterial->release();
}

void OpenGLMesh::destroy() {
    if (m_vao) delete m_vao;
    if (m_vbo) delete m_vbo;
    if (m_ebo) delete m_ebo;
    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
}

void OpenGLMesh::setSizeFixed(bool sizeFixed) {
    m_sizeFixed = sizeFixed;
}

void OpenGLMesh::setPickingID(uint id) {
    m_pickingID = id;
}

void OpenGLMesh::childEvent(QChildEvent * e) {
    if (e->removed()) {
        if (e->child() == m_openGLMaterial)
            m_openGLMaterial = 0;
    }
}

void OpenGLMesh::materialChanged(Material * material) {
    if (material == 0)
        m_openGLMaterial = 0;
    else
        m_openGLMaterial = new OpenGLMaterial(material);
}

void OpenGLMesh::geometryChanged(const QVector<Vertex>&, const QVector<uint32_t>&) {
    this->create();
}

void OpenGLMesh::hostDestroyed(QObject *) {
    // Commit suicide
    delete this;
}
