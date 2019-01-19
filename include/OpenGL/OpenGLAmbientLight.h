#pragma once

#include <OpenGL/Common.h>

class OpenGLAmbientLight: public QObject {
    Q_OBJECT

public:
    OpenGLAmbientLight(AmbientLight* light);

    AmbientLight* host() const;
    void bind(QOpenGLShaderProgram* shader, int indx);

private:
    AmbientLight* m_host;

private slots:
    void hostDestroyed(QObject* host);
};
