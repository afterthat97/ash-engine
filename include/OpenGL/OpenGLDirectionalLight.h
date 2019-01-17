#pragma once

#include <OpenGL/Common.h>

class OpenGLDirectionalLight: public QObject {
    Q_OBJECT

public:
    OpenGLDirectionalLight(DirectionalLight* light);
    // TODO: Copy constructor

    DirectionalLight* host() const;
    void bind(QOpenGLShaderProgram* shader, int indx);

private:
    DirectionalLight* m_host;

private slots:
    void hostDestroyed(QObject* host);
};
