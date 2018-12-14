#pragma once

#include <UI/Common.h>
#include <OpenGL/OpenGLRenderer.h>
#include <Generic/Scene.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = Q_NULLPTR);
    ~OpenGLWidget();
    QMatrix4x4 getProjectionMatrix();

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    OpenGLRenderer * renderer;
    map<int, bool> keyPressed;
    QPoint lastCursorPos;
    QTime timer;
    uint32_t totFrames;
    void processInput();
};
