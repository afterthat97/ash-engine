#pragma once

#include <OpenGL/Common.h>
#include <Generic/Mesh.h>

class OpenGLMesh {
public:
    OpenGLMesh();
    OpenGLMesh(Mesh* mesh);
    ~OpenGLMesh();

    void create(Mesh* mesh);
    void bind();
    void render();
    void release();
    
    void setVertexAttribPointer(GLuint indx, GLuint size, GLsizei stride, const void* ptr);
    QMatrix4x4 getModelMatrix();

private:
    Mesh* genericMesh;
    QOpenGLVertexArrayObject * vao;
    QOpenGLBuffer * vbo, * ebo;
};