#pragma once

#include <OpenGL/Common.h>

class OpenGLTexture: public QObject {
    Q_OBJECT

public:
    OpenGLTexture(Texture* texture);
    // TODO: Copy constructor
    ~OpenGLTexture();

    void bind(QOpenGLShaderProgram* shader);
    void release();

private:
    Texture* m_host;
    QOpenGLTexture *m_openGLTexture;

private slots:
    void imageChanged(const QImage& image);
    void hostDestroyed(QObject* host);
};
