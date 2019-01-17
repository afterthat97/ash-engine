#pragma once

#include <OpenGL/Common.h>

class OpenGLMaterial: public QObject {
    Q_OBJECT

public:
    OpenGLMaterial(Material* material);
    // TODO: Copy constructor
    
    Material* host() const;
    void bind(QOpenGLShaderProgram* shader);
    void release();

private:
    Material* m_host;

private slots:
    void hostDestroyed(QObject* host);
};
