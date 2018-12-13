#include <OpenGL/OpenGLTexture.h>

OpenGLTexture::OpenGLTexture() {
    openGLTexture = NULL;
}

OpenGLTexture::OpenGLTexture(Texture * texture) {
    create(texture);
}

OpenGLTexture::~OpenGLTexture() {
    if (openGLTexture) delete openGLTexture;
}

void OpenGLTexture::create(Texture * texture) {
    openGLTexture = new QOpenGLTexture(*texture->getImage());
    openGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    openGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    openGLTexture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLTexture::bind(GLuint indx) {
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glActiveTexture(GL_TEXTURE0 + indx);
    glFuncs->glBindTexture(GL_TEXTURE_2D, openGLTexture->textureId());
}

void OpenGLTexture::release() {
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
}
