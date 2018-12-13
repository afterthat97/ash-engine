#pragma once

#include <OpenGL/Common.h>
#include <Generic/Texture.h>

class OpenGLTexture {
public:
    OpenGLTexture();
    OpenGLTexture(Texture* texture);
    ~OpenGLTexture();

    void create(Texture* texture);
    void bind(GLuint indx);
    void release();

private:
    QOpenGLTexture *openGLTexture;
};