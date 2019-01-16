#include <OpenGL/OpenGLSpotLight.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLMesh.h>
#include <IO/ModelLoader.h>

OpenGLSpotLight::OpenGLSpotLight(SpotLight * light) {
    m_host = light;
    ModelLoader loader;
    m_flashLightMesh = loader.loadFromFile(":/resources/shapes/SpotLight.fbx")->childMeshes()[0];
    if (m_flashLightMesh->material())
        delete m_flashLightMesh->material();
    m_flashLightMesh->setPosition(m_host->position());
    m_flashLightMesh->setRotation(QQuaternion::rotationTo(QVector3D(0, -1, 0), m_host->direction()));
    connect(m_host, SIGNAL(positionChanged(QVector3D)), m_flashLightMesh, SLOT(setPosition(QVector3D)));
    connect(m_host, SIGNAL(directionChanged(QVector3D)), this, SLOT(directionChanged(QVector3D)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

OpenGLSpotLight::~OpenGLSpotLight() {
    delete m_flashLightMesh;
}

SpotLight * OpenGLSpotLight::host() const {
    return m_host;
}

void OpenGLSpotLight::bind(QOpenGLShaderProgram * shader, int indx) {
    if (!m_host->enabled()) return;
    QString pref = "spotLight[" + QString::number(indx) + "]";
    shader->bind();
    shader->setUniformValue((pref + ".color").toLocal8Bit().data(), m_host->color() * m_host->intensity());
    shader->setUniformValue((pref + ".pos").toLocal8Bit().data(), m_host->position());
    shader->setUniformValue((pref + ".direction").toLocal8Bit().data(), m_host->direction());
    shader->setUniformValue((pref + ".innerCutOff").toLocal8Bit().data(), (float) cos(m_host->innerCutOff() * M_PI / 180.0));
    shader->setUniformValue((pref + ".outerCutOff").toLocal8Bit().data(), (float) cos(m_host->outerCutOff() * M_PI / 180.0));
    shader->setUniformValue((pref + ".enableAttenuation").toLocal8Bit().data(), (float) m_host->enableAttenuation());
    shader->setUniformValue((pref + ".attenuationQuadratic").toLocal8Bit().data(), m_host->attenuationQuadratic());
    shader->setUniformValue((pref + ".attenuationLinear").toLocal8Bit().data(), m_host->attenuationLinear());
    shader->setUniformValue((pref + ".attenuationConstant").toLocal8Bit().data(), m_host->attenuationConstant());
}

void OpenGLSpotLight::render(QOpenGLShaderProgram * shader) {
    if (!m_host->enabled()) return;
    shader->bind();
    shader->setUniformValue("color", m_host->color());
    OpenGLManager<Mesh, OpenGLMesh>::currentManager()->getOpenGLObject(m_flashLightMesh)->render(shader);
}

void OpenGLSpotLight::directionChanged(QVector3D direction) {
    m_flashLightMesh->setRotation(QQuaternion::rotationTo(QVector3D(0, -1, 0), direction));
}

void OpenGLSpotLight::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<SpotLight, OpenGLSpotLight>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
