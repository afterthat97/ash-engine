#pragma once

#include <OpenGL/Common.h>

class OpenGLRenderer: public QObject {
    Q_OBJECT

public:
    OpenGLRenderer(QObject* parent = 0);
    // TODO: Copy constructor

    void loadShaders();
    void render(Scene* scene);

private:
    QOpenGLShaderProgram * basicShader, *phongShader;

    QOpenGLShaderProgram * loadShaderFromFile(QString vertexShaderFile, QString fragmentShaderFile, QString geometryShaderFile = "");
};
