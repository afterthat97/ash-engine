#include <OpenGL/OpenGLDirectionalLight.h>
#include <OpenGL/OpenGLManager.h>

OpenGLDirectionalLight::OpenGLDirectionalLight(DirectionalLight * light) {
    m_host = light;
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

DirectionalLight * OpenGLDirectionalLight::host() const {
    return m_host;
}

void OpenGLDirectionalLight::bind(QOpenGLShaderProgram * shader, int indx) {
    if (!m_host->enabled()) return;
    QString pref = "directionalLight[" + QString::number(indx) + "]";
    shader->bind();
    shader->setUniformValue((pref + ".color").toLocal8Bit().data(), m_host->color() * m_host->intensity());
    shader->setUniformValue((pref + ".direction").toLocal8Bit().data(), m_host->direction());
}

void OpenGLDirectionalLight::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<DirectionalLight, OpenGLDirectionalLight>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
