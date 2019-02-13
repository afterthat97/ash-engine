#pragma once

#include <OpenGLScene.h>

class OpenGLRenderer: public QObject {
    Q_OBJECT

public:
    OpenGLRenderer(QObject* parent = 0);
    OpenGLRenderer(const OpenGLRenderer& renderer);

    bool hasLog();
    QString log();

    bool reloadShaders();
    void reloadFrameBuffers();

    uint32_t pickingPass(OpenGLScene* openGLScene, QPoint cursorPos);
    void render(OpenGLScene* openGLScene);

private:
    QString m_log;
    QOpenGLShaderProgram *m_basicShader, *m_pickingShader, *m_phongShader;
    QOpenGLFramebufferObject *m_pickingPassFBO;

    QOpenGLShaderProgram * loadShaderFromFile(
        QString vertexShaderFilePath,
        QString fragmentShaderFilePath,
        QString geometryShaderFilePath = "");
};
