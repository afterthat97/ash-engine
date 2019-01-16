#include <OpenGL/OpenGLAmbientLight.h>
#include <OpenGL/OpenGLManager.h>

OpenGLAmbientLight::OpenGLAmbientLight(AmbientLight * light) {
    m_host = light;
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

AmbientLight * OpenGLAmbientLight::host() const {
    return m_host;
}

void OpenGLAmbientLight::bind(QOpenGLShaderProgram * shader, int indx) {
    if (!m_host->enabled()) return;
    QString pref = "ambientLight[" + QString::number(indx) + "]";
    shader->bind();
    shader->setUniformValue((pref + ".color").toLocal8Bit().data(), m_host->color() * m_host->intensity());
}

void OpenGLAmbientLight::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<AmbientLight, OpenGLAmbientLight>::currentManager()->removeOpenGLObject(m_host);
    
    // Commit suicide
    delete this;
}
