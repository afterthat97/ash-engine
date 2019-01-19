#pragma once

#include <OpenGL/Common.h>

class OpenGLSpotLight: public QObject {
    Q_OBJECT

public:
    OpenGLSpotLight(SpotLight* light);
    ~OpenGLSpotLight();

    SpotLight* host() const;
    void bind(QOpenGLShaderProgram* shader, int indx);
    void render(QOpenGLShaderProgram* shader);

private:
    SpotLight* m_host;
    Mesh* m_flashLightMesh;

private slots:
    void directionChanged(QVector3D direction);
    void hostDestroyed(QObject* host);
};
