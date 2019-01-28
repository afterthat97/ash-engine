#include <OpenGL/OpenGLScene.h>
#include <OpenGL/OpenGLConfig.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLGridline.h>
#include <OpenGL/OpenGLAmbientLight.h>
#include <OpenGL/OpenGLDirectionalLight.h>
#include <OpenGL/OpenGLPointLight.h>
#include <OpenGL/OpenGLSpotLight.h>
#include <OpenGL/OpenGLModel.h>

OpenGLScene::OpenGLScene(Scene * scene) {
    m_host = scene;
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

Scene * OpenGLScene::host() const {
    return m_host;
}

void OpenGLScene::bindCamera(QOpenGLShaderProgram * shader) {
    shader->bind();
    shader->setUniformValue("projMat", m_host->camera()->projectionMatrix());
    shader->setUniformValue("viewMat", m_host->camera()->viewMatrix());
    shader->setUniformValue("viewPos", m_host->camera()->position());
}

void OpenGLScene::renderGridlines(QOpenGLShaderProgram * shader) {
    for (int i = 0; i < m_host->gridlines().size(); i++) {
        Gridline* gridline = m_host->gridlines()[i];
        OpenGLManager<Gridline, OpenGLGridline>::currentManager()->getOpenGLObject(gridline)->render(shader);
    }
}

void OpenGLScene::bindLights(QOpenGLShaderProgram * shader) {
    int ambientLightNum = 0, directionalLightNum = 0, pointLightNum = 0, spotLightNum = 0;

    for (int i = 0; i < m_host->ambientLights().size(); i++)
        OpenGLManager<AmbientLight, OpenGLAmbientLight>::currentManager()->getOpenGLObject(m_host->ambientLights()[i])->bind(shader, ambientLightNum++);

    for (int i = 0; i < m_host->directionalLights().size(); i++)
        OpenGLManager<DirectionalLight, OpenGLDirectionalLight>::currentManager()->getOpenGLObject(m_host->directionalLights()[i])->bind(shader, directionalLightNum++);

    for (int i = 0; i < m_host->pointLights().size(); i++)
        OpenGLManager<PointLight, OpenGLPointLight>::currentManager()->getOpenGLObject(m_host->pointLights()[i])->bind(shader, pointLightNum++);

    for (int i = 0; i < m_host->spotLights().size(); i++)
        OpenGLManager<SpotLight, OpenGLSpotLight>::currentManager()->getOpenGLObject(m_host->spotLights()[i])->bind(shader, spotLightNum++);

    shader->bind();
    shader->setUniformValue("ambientLightNum", ambientLightNum);
    shader->setUniformValue("directionalLightNum", directionalLightNum);
    shader->setUniformValue("pointLightNum", pointLightNum);
    shader->setUniformValue("spotLightNum", spotLightNum);
}

void OpenGLScene::renderLights(QOpenGLShaderProgram * shader) {
    for (int i = 0; i < m_host->pointLights().size(); i++)
        OpenGLManager<PointLight, OpenGLPointLight>::currentManager()->getOpenGLObject(m_host->pointLights()[i])->render(shader);

    for (int i = 0; i < m_host->spotLights().size(); i++)
        OpenGLManager<SpotLight, OpenGLSpotLight>::currentManager()->getOpenGLObject(m_host->spotLights()[i])->render(shader);
}

void OpenGLScene::renderModels(QOpenGLShaderProgram * shader) {
    for (int i = 0; i < m_host->models().size(); i++) {
        Model* model = m_host->models()[i];
        OpenGLManager<Model, OpenGLModel>::currentManager()->getOpenGLObject(model)->render(shader);
    }
}

void OpenGLScene::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<Scene, OpenGLScene>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
