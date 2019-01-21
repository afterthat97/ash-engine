#include <OpenGL/OpenGLTexture.h>
#include <OpenGL/OpenGLManager.h>

OpenGLTexture::OpenGLTexture(Texture * texture) {
    m_host = texture;
    m_openGLTexture = new QOpenGLTexture(texture->image());
    m_openGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_openGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_openGLTexture->setWrapMode(QOpenGLTexture::Repeat);
    connect(m_host, SIGNAL(imageChanged(QImage)), this, SLOT(imageChanged(QImage)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

OpenGLTexture::~OpenGLTexture() {
    delete m_openGLTexture;
}

void OpenGLTexture::bind(QOpenGLShaderProgram* shader) {
    if (!m_host->enabled()) return;
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    if (m_host->textureType() == Texture::Diffuse) { // Diffuse map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 0);
        glFuncs->glBindTexture(GL_TEXTURE_2D, m_openGLTexture->textureId());
        shader->setUniformValue("material.diffuseMap", 0);
    } else if (m_host->textureType() == Texture::Specular) { // Specular map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 1);
        glFuncs->glBindTexture(GL_TEXTURE_2D, m_openGLTexture->textureId());
        shader->setUniformValue("material.specularMap", 1);
    } else if (m_host->textureType() == Texture::Bump) { // Bump map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 2);
        glFuncs->glBindTexture(GL_TEXTURE_2D, m_openGLTexture->textureId());
        shader->setUniformValue("material.bumpMap", 2);
    }
}

void OpenGLTexture::release() {
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    if (m_host->textureType() == Texture::Diffuse) { // Diffuse map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 0);
        glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
    } else if (m_host->textureType() == Texture::Specular) { // Specular map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 1);
        glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
    } else if (m_host->textureType() == Texture::Bump) { // Bump map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 2);
        glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void OpenGLTexture::imageChanged(const QImage& image) {
    delete m_openGLTexture;
    m_openGLTexture = new QOpenGLTexture(image);
    m_openGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_openGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_openGLTexture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLTexture::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<Texture, OpenGLTexture>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
