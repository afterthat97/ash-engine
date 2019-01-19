#pragma once

#include <OpenGL/Common.h>

class OpenGLScene: public QObject {
    Q_OBJECT

public:
    OpenGLScene(Scene* scene);
    
    Scene* host() const;
    void bindCamera(QOpenGLShaderProgram* shader);
    void renderGridlines(QOpenGLShaderProgram* shader);
    void bindLights(QOpenGLShaderProgram* shader);
    void renderLights(QOpenGLShaderProgram* shader);
    void renderModels(QOpenGLShaderProgram* shader);

private:
    Scene* m_host;

private slots:
    void hostDestroyed(QObject* host);
};
