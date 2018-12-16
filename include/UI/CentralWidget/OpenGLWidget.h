#pragma once

#include <UI/Common.h>
#include <OpenGL/OpenGLRenderer.h>
#include <Generic/Scene.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget * parent = Q_NULLPTR);
    ~OpenGLWidget();
    void setFPSLimit(uint32_t limit);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void FPSChanged(uint32_t FPS);

private:
    OpenGLRenderer * renderer;
    QTime time;
    QTimer *timer;
    uint32_t totFrames;
    map<int, bool> keyPressed;
    QPoint lastCursorPos;

    void processUserInput();
};
