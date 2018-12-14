#include <OpenGL/OpenGLMesh.h>
#include <Generic/Mesh.h>

OpenGLMesh::OpenGLMesh() {
    vao = NULL;
    vbo = NULL;
    ebo = NULL;
}

OpenGLMesh::OpenGLMesh(Mesh * mesh) {
    create(mesh);
}

OpenGLMesh::~OpenGLMesh() {
    if (vao) delete vao;
    if (vbo) delete vbo;
    if (ebo) delete ebo;
}

void OpenGLMesh::create(Mesh * mesh) {
    genericMesh = mesh;
    vao = new QOpenGLVertexArrayObject;
    vao->create();
    vao->bind();

    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(&mesh->getVertices()[0], sizeof(Vertex) * mesh->getVertices().size());

    ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo->create();
    ebo->bind();
    ebo->allocate(&mesh->getIndices()[0], sizeof(uint32_t) * mesh->getIndices().size());

    vao->release();
}

void OpenGLMesh::bind() {
    vao->bind();
}

void OpenGLMesh::render() {
    if (!genericMesh->isVisible()) return;
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    if (genericMesh->getType() == Mesh::Triangle)
        glFuncs->glDrawElements(GL_TRIANGLES, (GLsizei) genericMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
    else if (genericMesh->getType() == Mesh::Line)
        glFuncs->glDrawElements(GL_LINES, (GLsizei) genericMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
    else
        glFuncs->glDrawElements(GL_POINTS, (GLsizei) genericMesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

void OpenGLMesh::release() {
    vao->release();
}

void OpenGLMesh::setVertexAttribPointer(GLuint indx, GLuint size, GLsizei stride, const void * ptr) {
    bind();
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glEnableVertexAttribArray(indx);
    glFuncs->glVertexAttribPointer(indx, size, GL_FLOAT, GL_FALSE, stride, ptr);
    release();
}

QMatrix4x4 OpenGLMesh::getModelMatrix() {
    return genericMesh->getModelMatrix();
}
