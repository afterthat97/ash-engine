#pragma once

#include <OpenGL/Common.h>

class OpenGLUniformBufferObject {

public:
    OpenGLUniformBufferObject();
    ~OpenGLUniformBufferObject();

    GLuint bufferId() const;

    bool create();
    void allocate(int indx, const void *data, int count);
    void destroy();

    void bind();
    void write(int offset, const void* data, int count);
    void release();

private:
    GLuint m_id;
    QOpenGLFunctions_3_3_Core * glFuncs;
};