#pragma once

#include <OpenGL/Common.h>

class OpenGLMesh: public QObject {
    Q_OBJECT

public:
    OpenGLMesh(Mesh* mesh);
    ~OpenGLMesh();

    Mesh* host() const;
    void render(QOpenGLShaderProgram* shader);

private:
    Mesh* m_host;
    QOpenGLVertexArrayObject * m_vao;
    QOpenGLBuffer * m_vbo, *m_ebo;

private slots:
    void geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
    void hostDestroyed(QObject* host);
};
