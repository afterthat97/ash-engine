#pragma once

#include <Texture.h>

class OpenGLTexture: public QObject {
    Q_OBJECT

public:
    OpenGLTexture(Texture* texture);
    ~OpenGLTexture();

    void bind();
    void release();

private:
    Texture* m_host;
    QOpenGLTexture *m_openGLTexture;

private slots:
    void imageChanged(const QImage& image);
    void hostDestroyed(QObject* host);
};
