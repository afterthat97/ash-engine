#include <OpenGLRenderer.h>

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

bool OpenGLRenderer::hasErrorLog() {
    return m_log != "";
}

QString OpenGLRenderer::errorLog() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}

bool OpenGLRenderer::reloadShaders() {
    if (m_pickingShader) {
        delete m_pickingShader;
        m_pickingShader = 0;
    }
    if (m_basicShader) {
        delete m_basicShader;
        m_basicShader = 0;
    }
    if (m_phongShader) {
        delete m_phongShader;
        m_phongShader = 0;
    }

    m_pickingShader = loadShaderFromFile(":/resources/shaders/picking.vert", ":/resources/shaders/picking.frag");
    m_basicShader = loadShaderFromFile(":/resources/shaders/basic.vert", ":/resources/shaders/basic.frag");
    m_phongShader = loadShaderFromFile(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");

    if (m_phongShader) {
        m_phongShader->bind();
        m_phongShader->setUniformValue("diffuseMap", 0);
        m_phongShader->setUniformValue("specularMap", 1);
        m_phongShader->setUniformValue("bumpMap", 2);
    }

    return m_pickingShader && m_basicShader && m_phongShader;
}

void OpenGLRenderer::reloadFrameBuffers() {
    if (m_pickingPassFBO)
        delete m_pickingPassFBO;
    int data[4];
    glGetIntegerv(GL_VIEWPORT, data);
    m_pickingPassFBO = new QOpenGLFramebufferObject(data[2], data[3], QOpenGLFramebufferObject::CombinedDepthStencil);
}

uint32_t OpenGLRenderer::pickingPass(OpenGLScene * openGLScene, QPoint cursorPos) {
    if (m_pickingPassFBO == 0) reloadFrameBuffers();

    int data[4];
    glGetIntegerv(GL_VIEWPORT, data);
    if (data[2] != m_pickingPassFBO->width() || data[3] != m_pickingPassFBO->height())
        reloadFrameBuffers();

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
#ifdef DEBUG_OUTPUT
        dout << "Failed to load glsl definitions";
#endif
        return 0;
    }
    if (!glslUBOFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to load glsl UBOs.";
#ifdef DEBUG_OUTPUT
        dout << "Failed to load glsl UBOs";
#endif
        return 0;
    }

    QFile vertexShaderFile(vertexShaderFilePath);
    QFile fragmentShaderFile(fragmentShaderFilePath);
    QFile geometryShaderFile(geometryShaderFilePath);
    if (!vertexShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to open file: " + vertexShaderFilePath;
#ifdef DEBUG_OUTPUT
        dout << "Failed to open file: " + vertexShaderFilePath;
#endif
        return 0;
    }
    if (!fragmentShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to open file: " + fragmentShaderFilePath;
#ifdef DEBUG_OUTPUT
        dout << "Failed to open file: " + fragmentShaderFilePath;
#endif
        return 0;
    }
    if (geometryShaderFilePath != "" && !geometryShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_log += "Failed to open file: " + geometryShaderFilePath;
#ifdef DEBUG_OUTPUT
        dout << "Failed to open file: " + geometryShaderFilePath;
#endif
        return 0;
    }

    QByteArray glslDefineCode = glslDefineFile.readAll();
    QByteArray glslUBOCode = glslUBOFile.readAll();

    QByteArray vertexShaderCode = "#version 330 core\n"
        + glslDefineCode + "\n"
        + glslUBOCode + "\n"
        + vertexShaderFile.readAll();
    QByteArray fragmentShaderCode = "#version 330 core\n"
        + glslDefineCode + "\n"
        + glslUBOCode + "\n"
        + fragmentShaderFile.readAll();
    QByteArray geometryShaderCode = "#version 330 core\n"
        + glslDefineCode + "\n"
        + glslUBOCode + "\n"
        + (geometryShaderFilePath != "" ? geometryShaderFile.readAll() : "");

    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram(this);
    if (!shader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderCode)) {
        m_log += "Failed to compile vertex shader: " + shader->log();
#ifdef DEBUG_OUTPUT
        dout << "Failed to compile vertex shader: " + shader->log();
#endif
        return 0;
    }
    if (!shader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderCode)) {
        m_log += "Failed to compile fragment shader:" + shader->log();
#ifdef DEBUG_OUTPUT
        dout << "Failed to compile fragment shader:" + shader->log();
#endif
        return 0;
    }
    if (geometryShaderFilePath != "" && !shader->addShaderFromSourceCode(QOpenGLShader::Geometry, geometryShaderCode)) {
        m_log += "Failed to compile geometry shader:" + shader->log();
#ifdef DEBUG_OUTPUT
        dout << "Failed to compile geometry shader:" + shader->log();
#endif
        return 0;
    }
    if (!shader->link()) {
        m_log += "Failed to link shaders:" + shader->log();
#ifdef DEBUG_OUTPUT
        dout << "Failed to link shaders:" + shader->log();
#endif
        return 0;
    }
    OpenGLUniformBufferObject::bindUniformBlock(shader);
    return shader;
}
