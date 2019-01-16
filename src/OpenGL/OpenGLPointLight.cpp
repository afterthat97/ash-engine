#include <OpenGL/OpenGLPointLight.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLMesh.h>
#include <IO/ModelLoader.h>

OpenGLPointLight::OpenGLPointLight(PointLight * light) {
    m_host = light;
    ModelLoader loader;
    m_lightBulbMesh = loader.loadFromFile(":/resources/shapes/PointLight.fbx")->childMeshes()[0];
    if (m_lightBulbMesh->material())
        delete m_lightBulbMesh->material();
    m_lightBulbMesh->setPosition(m_host->position());
    connect(m_host, SIGNAL(positionChanged(QVector3D)), m_lightBulbMesh, SLOT(setPosition(QVector3D)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

OpenGLPointLight::~OpenGLPointLight() {
    delete m_lightBulbMesh;
}

PointLight * OpenGLPointLight::host() const {
    return m_host;
}

void OpenGLPointLight::bind(QOpenGLShaderProgram * shader, int indx) {
    if (!m_host->enabled()) return;
    QString pref = "pointLight[" + QString::number(indx) + "]";
    shader->bind();
    shader->setUniformValue((pref + ".color").toLocal8Bit().data(), m_host->color() * m_host->intensity());
    shader->setUniformValue((pref + ".pos").toLocal8Bit().data(), m_host->position());
    shader->setUniformValue((pref + ".enableAttenuation").toLocal8Bit().data(), (float) m_host->enableAttenuation());
    shader->setUniformValue((pref + ".attenuationQuadratic").toLocal8Bit().data(), m_host->attenuationQuadratic());
    shader->setUniformValue((pref + ".attenuationLinear").toLocal8Bit().data(), m_host->attenuationLinear());
    shader->setUniformValue((pref + ".attenuationConstant").toLocal8Bit().data(), m_host->attenuationConstant());
}

void OpenGLPointLight::render(QOpenGLShaderProgram * shader) {
    if (!m_host->enabled()) return;
    shader->bind();
    shader->setUniformValue("color", m_host->color());
    OpenGLManager<Mesh, OpenGLMesh>::currentManager()->getOpenGLObject(m_lightBulbMesh)->render(shader);
}

void OpenGLPointLight::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<PointLight, OpenGLPointLight>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
