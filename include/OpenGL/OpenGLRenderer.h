#pragma once

#include <OpenGL/Common.h>

class OpenGLRenderer: public QObject {
    Q_OBJECT

public:
    OpenGLRenderer(QObject* parent = 0);
    OpenGLRenderer(const OpenGLRenderer& renderer);

    bool hasLog();
    QString log();

    bool loadShaders();
    void render(Scene* scene);

private:
    QString m_log;
    QOpenGLShaderProgram * m_basicShader, *m_phongShader;

    QOpenGLShaderProgram * loadShaderFromFile(QString vertexShaderFile, QString fragmentShaderFile, QString geometryShaderFile = "");
};
