#pragma once

#include <OpenGL/Common.h>

class OpenGLGridline: public QObject {
    Q_OBJECT

public:
    OpenGLGridline(Gridline* gridline);
    // TODO: Copy constructor
    ~OpenGLGridline();

    Gridline* host() const;
    void render(QOpenGLShaderProgram* shader);

private:
    Gridline* m_host;
    Mesh* m_gridlineMesh;
    bool m_needUpdate;

    void update();

private slots:
    void hostDestroyed(QObject* host);
    void xArgumentsChanged(QVector3D xargs);
    void yArgumentsChanged(QVector3D yargs);
    void zArgumentsChanged(QVector3D zargs);
};
