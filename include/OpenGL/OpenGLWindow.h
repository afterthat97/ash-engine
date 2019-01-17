#pragma once

#include <OpenGL/Common.h>
#include <OpenGL/OpenGLRenderer.h>

// TODO: Try to create OpenGLWindow inside a widget

class OpenGLWindow: public QOpenGLWindow, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWindow();
    // TODO: Copy constructor
    ~OpenGLWindow();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    OpenGLRenderer * renderer;
    QHash<int, bool> keyPressed;
    QPoint lastCursorPos;

    int totFrames;
    QTime time;

    void processUserInput();
};
