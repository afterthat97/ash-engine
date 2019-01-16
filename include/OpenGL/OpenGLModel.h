#pragma once

#include <OpenGL/Common.h>

class OpenGLModel: public QObject {
    Q_OBJECT

public:
    OpenGLModel(Model* model);

    Model* host() const;
    void render(QOpenGLShaderProgram* shader);

private:
    Model* m_host;

private slots:
    void hostDestroyed(QObject* host);
};