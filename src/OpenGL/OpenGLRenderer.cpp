#include <OpenGL/OpenGLRenderer.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLConfig.h>
#include <OpenGL/OpenGLScene.h>

OpenGLRenderer::OpenGLRenderer(QObject* parent): QObject(parent) {
    basicShader = phongShader = 0;
}

void OpenGLRenderer::loadShaders() {
    basicShader = loadShaderFromFile(":/resources/shaders/basic.vert", ":/resources/shaders/basic.frag");
    basicShader->bindAttributeLocation("position", 0);
    phongShader = loadShaderFromFile(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");
    phongShader->bindAttributeLocation("position", 0);
    phongShader->bindAttributeLocation("normal", 1);
    phongShader->bindAttributeLocation("tangent", 2);
    phongShader->bindAttributeLocation("bitangent", 3);
    phongShader->bindAttributeLocation("texCoords", 4);
}

void OpenGLRenderer::render(Scene* scene) {
    if (basicShader == 0 || phongShader == 0)
        loadShaders();
    OpenGLScene* openGLScene = OpenGLManager<Scene, OpenGLScene>::currentManager()->getOpenGLObject(scene);
    openGLScene->bindCamera(basicShader);
    openGLScene->bindCamera(phongShader);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(OpenGLConfig::getBackgroundColor()[0], OpenGLConfig::getBackgroundColor()[1], OpenGLConfig::getBackgroundColor()[2], 1.0f);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    openGLScene->renderGridlines(basicShader);
    openGLScene->renderLights(basicShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    openGLScene->bindLights(phongShader);
    openGLScene->renderModels(phongShader);
}

QOpenGLShaderProgram * OpenGLRenderer::loadShaderFromFile(QString vertexShaderFile, QString fragmentShaderFile, QString geometryShaderFile) {
    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram(this);
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile)) {
        qWarning() << "Failed to compile vertex shader:" << shader->log();
        return 0;
    }
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile)) {
        qWarning() << "Failed to compile fragment shader:" << shader->log();
        return 0;
    }
    if (geometryShaderFile != "" && !shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryShaderFile)) {
        qWarning() << "Failed to compile geometry shader:" << shader->log();
        return 0;
    }
    if (!shader->link()) {
        qWarning() << "Failed to link shaders:" << shader->log();
        return 0;
    }
    return shader;
}
