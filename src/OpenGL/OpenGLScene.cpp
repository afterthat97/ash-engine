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
    for (int i = 0; i < m_host->lights().size(); i++) {
        if (!m_host->lights()[i]->enabled()) continue;
        if (SpotLight* light = qobject_cast<SpotLight*>(m_host->lights()[i])) {
            OpenGLManager<SpotLight, OpenGLSpotLight>::currentManager()->getOpenGLObject(light)->bind(shader, spotLightNum++);
        } else if (AmbientLight* light = qobject_cast<AmbientLight*>(m_host->lights()[i])) {
            OpenGLManager<AmbientLight, OpenGLAmbientLight>::currentManager()->getOpenGLObject(light)->bind(shader, ambientLightNum++);
        } else if (DirectionalLight* light = qobject_cast<DirectionalLight*>(m_host->lights()[i])) {
            OpenGLManager<DirectionalLight, OpenGLDirectionalLight>::currentManager()->getOpenGLObject(light)->bind(shader, directionalLightNum++);
        } else if (PointLight* light = qobject_cast<PointLight*>(m_host->lights()[i])) {
            OpenGLManager<PointLight, OpenGLPointLight>::currentManager()->getOpenGLObject(light)->bind(shader, pointLightNum++);
        }
    }
    shader->bind();
    shader->setUniformValue("ambientLightNum", ambientLightNum);
    shader->setUniformValue("directionalLightNum", directionalLightNum);
    shader->setUniformValue("pointLightNum", pointLightNum);
    shader->setUniformValue("spotLightNum", spotLightNum);
}

void OpenGLScene::renderLights(QOpenGLShaderProgram * shader) {
    for (int i = 0; i < m_host->lights().size(); i++)
        if (SpotLight* light = qobject_cast<SpotLight*>(m_host->lights()[i])) {
            OpenGLManager<SpotLight, OpenGLSpotLight>::currentManager()->getOpenGLObject(light)->render(shader);
        } else if (PointLight* light = qobject_cast<PointLight*>(m_host->lights()[i])) {
            OpenGLManager<PointLight, OpenGLPointLight>::currentManager()->getOpenGLObject(light)->render(shader);
        }
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
