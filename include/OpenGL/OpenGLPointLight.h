#pragma once

#include <OpenGL/Common.h>

class OpenGLPointLight: public QObject {
    Q_OBJECT

public:
    OpenGLPointLight(PointLight* light);
    // TODO: Copy constructor
    ~OpenGLPointLight();

    PointLight* host() const;
    void bind(QOpenGLShaderProgram* shader, int indx);
    void render(QOpenGLShaderProgram* shader);

private:
    PointLight* m_host;
    Mesh* m_lightBulbMesh;

private slots:
    void hostDestroyed(QObject* host);
};
