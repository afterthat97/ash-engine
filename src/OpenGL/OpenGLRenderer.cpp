#include <OpenGL/OpenGLRenderer.h>

OpenGLRenderer::OpenGLRenderer(QObject* parent): QObject(0) {
    m_log = "";
    m_pickingShader = m_basicShader = m_phongShader = 0;
    m_pickingPassFBO = 0;
    setParent(parent);
}

OpenGLRenderer::OpenGLRenderer(const OpenGLRenderer &) {
    m_log = "";
    m_basicShader = m_phongShader = 0;
    m_pickingPassFBO = 0;
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
    m_pickingShader = loadShaderFromFile(":/resources/shaders/picking.vert", ":/resources/shaders/picking.frag");
    m_basicShader = loadShaderFromFile(":/resources/shaders/basic.vert", ":/resources/shaders/basic.frag");
    m_phongShader = loadShaderFromFile(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");

    if (m_phongShader) {
        m_phongShader->bind();
        m_phongShader->setUniformValue("diffuseMap", 0);
        m_phongShader->setUniformValue("specularMap", 1);
        m_phongShader->setUniformValue("bumpMap", 2);
    }
    
    return m_basicShader != 0 && m_phongShader != 0;
}

uint32_t OpenGLRenderer::pickingPass(OpenGLScene * openGLScene, QPoint cursorPos) {
    if (m_pickingPassFBO == 0) {
        int data[4];
        glGetIntegerv(GL_VIEWPORT, data);
        m_pickingPassFBO = new QOpenGLFramebufferObject(data[2], data[3], QOpenGLFramebufferObject::CombinedDepthStencil);
    }

    if (cursorPos.x() < 0 || cursorPos.y() < 0 || cursorPos.x() >= m_pickingPassFBO->width() || cursorPos.y() >= m_pickingPassFBO->height())
        return 0;

    m_pickingPassFBO->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (m_pickingShader) {
        m_pickingShader->bind();
        openGLScene->renderLights();
        openGLScene->renderModels();
        openGLScene->renderAxis();
    }

    m_pickingPassFBO->release();

    QColor rgb = m_pickingPassFBO->toImage().pixelColor(cursorPos);
    return rgb.red() + rgb.green() * 256 + rgb.blue() * 256 * 256;
}

void OpenGLRenderer::render(OpenGLScene* openGLScene) {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (m_basicShader) {
        m_basicShader->bind();
        openGLScene->renderGridlines();
        openGLScene->renderLights();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (m_phongShader) {
        m_phongShader->bind();
        openGLScene->renderModels();
    }
    if (m_basicShader) {
        m_basicShader->bind();
        openGLScene->renderAxis();
    }
}

QOpenGLShaderProgram * OpenGLRenderer::loadShaderFromFile(
    QString vertexShaderFilePath,
    QString fragmentShaderFilePath,
    QString geometryShaderFilePath) {
    QFile glslDefineFile(":/resources/shaders/define.glsl");
    QFile glslUBOFile(":/resources/shaders/ubo.glsl");
    if (!glslDefineFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to load glsl definitions.";
        return 0;
    }
    if (!glslUBOFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to load glsl UBOs.";
        return 0;
    }

    QFile vertexShaderFile(vertexShaderFilePath);
    QFile fragmentShaderFile(fragmentShaderFilePath);
    QFile geometryShaderFile(geometryShaderFilePath);
    if (!vertexShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to open file: " + vertexShaderFilePath;
        return 0;
    }
    if (!fragmentShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to open file: " + fragmentShaderFilePath;
        return 0;
    }
    if (geometryShaderFilePath != "" && !geometryShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to open file: " + geometryShaderFilePath;
        return 0;
    }

    QByteArray glslDefineCode = glslDefineFile.readAll();
    QByteArray glslUBOCode = glslUBOFile.readAll();

    QByteArray vertexShaderCode = "#version 420 core\n"
        + glslDefineCode + "\n"
        + glslUBOCode + "\n"
        + vertexShaderFile.readAll();
    QByteArray fragmentShaderCode = "#version 420 core\n"
        + glslDefineCode + "\n"
        + glslUBOCode + "\n"
        + fragmentShaderFile.readAll();
    QByteArray geometryShaderCode = "#version 420 core\n"
        + glslDefineCode + "\n"
        + glslUBOCode + "\n"
        + (geometryShaderFilePath != "" ? geometryShaderFile.readAll() : "");

    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram(this);
    if (!shader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderCode)) {
        m_log += "Failed to compile vertex shader: " + shader->log();
        return 0;
    }
    if (!shader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderCode)) {
        m_log += "Failed to compile fragment shader:" + shader->log();
        return 0;
    }
    if (geometryShaderFilePath != "" && !shader->addShaderFromSourceCode(QOpenGLShader::Geometry, geometryShaderCode)) {
        m_log += "Failed to compile geometry shader:" + shader->log();
        return 0;
    }
    if (!shader->link()) {
        m_log += "Failed to link shaders:" + shader->log();
        return 0;
    }
    return shader;
}
