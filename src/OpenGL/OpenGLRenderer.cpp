#include <OpenGL/OpenGLRenderer.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLScene.h>

OpenGLRenderer::OpenGLRenderer(QObject* parent): QObject(0) {
    m_log = "";
    m_basicShader = m_phongShader = 0;
    setParent(parent);
}

OpenGLRenderer::OpenGLRenderer(const OpenGLRenderer &) {
    m_log = "";
    m_basicShader = m_phongShader = 0;
}

bool OpenGLRenderer::hasLog() {
    return m_log != "";
}

QString OpenGLRenderer::log() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}

bool OpenGLRenderer::loadShaders() {
    m_basicShader = loadShaderFromFile(":/resources/shaders/basic.vert", ":/resources/shaders/basic.frag");
    m_phongShader = loadShaderFromFile(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");
    return m_basicShader != 0 && m_phongShader != 0;
}

void OpenGLRenderer::render(Scene* scene) {
    OpenGLScene* openGLScene = OpenGLManager<Scene, OpenGLScene>::currentManager()->getOpenGLObject(scene);
    if (m_basicShader) openGLScene->bindCamera(m_basicShader);
    if (m_phongShader) openGLScene->bindCamera(m_phongShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (m_basicShader) openGLScene->renderGridlines(m_basicShader);
    if (m_basicShader) openGLScene->renderLights(m_basicShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (m_phongShader) openGLScene->bindLights(m_phongShader);
    if (m_phongShader) openGLScene->renderModels(m_phongShader);
}

QOpenGLShaderProgram * OpenGLRenderer::loadShaderFromFile(QString vertexShaderFile, QString fragmentShaderFile, QString geometryShaderFile) {
    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram(this);
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile)) {
        m_log += "Failed to compile vertex shader: " + shader->log();
        return 0;
    }
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile)) {
        m_log += "Failed to compile fragment shader:" + shader->log();
        return 0;
    }
    if (geometryShaderFile != "" && !shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryShaderFile)) {
        m_log += "Failed to compile geometry shader:" + shader->log();
        return 0;
    }
    if (!shader->link()) {
        m_log += "Failed to link shaders:" + shader->log();
        return 0;
    }
    return shader;
}
