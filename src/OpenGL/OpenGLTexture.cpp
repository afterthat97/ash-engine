#include <OpenGLTexture.h>

OpenGLTexture::OpenGLTexture(Texture * texture) {
    m_host = texture;

    if (m_host->property("OpenGLTexturePointer").isValid()) {
        qFatal("Multiple OpenGLTexture instances bound to a single core Texture!");
        exit(-1);
    }
    m_host->setProperty("OpenGLTexturePointer", QVariant::fromValue(this));

    m_openGLTexture = new QOpenGLTexture(texture->image());
    m_openGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_openGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_openGLTexture->setWrapMode(QOpenGLTexture::Repeat);
    connect(m_host, SIGNAL(imageChanged(QImage)), this, SLOT(imageChanged(QImage)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

OpenGLTexture::~OpenGLTexture() {
    delete m_openGLTexture;
    m_host->setProperty("OpenGLTexturePointer", QVariant());
}

bool OpenGLTexture::bind() {
    if (!m_host->enabled()) return false;
    QOpenGLFunctions * glFuncs = QOpenGLContext::currentContext()->functions();
    if (m_host->textureType() == Texture::Diffuse) { // Diffuse map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 0);
        glFuncs->glBindTexture(GL_TEXTURE_2D, m_openGLTexture->textureId());
    } else if (m_host->textureType() == Texture::Specular) { // Specular map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 1);
        glFuncs->glBindTexture(GL_TEXTURE_2D, m_openGLTexture->textureId());
    } else if (m_host->textureType() == Texture::Bump) { // Bump map
        glFuncs->glActiveTexture(GL_TEXTURE0 + 2);
        glFuncs->glBindTexture(GL_TEXTURE_2D, m_openGLTexture->textureId());
    }
    return true;
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
    // Commit suicide
    delete this;
}
