#pragma once

#include <UI/Common.h>
#include <OpenGL/OpenGLRenderer.h>
#include <Generic/Scene.h>

class MainWindow;

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(MainWindow* mainWindow, QWidget * parent = Q_NULLPTR);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    MainWindow * mainWindow;
    OpenGLRenderer * renderer;

    QTime time;
    QTimer *timer;
    uint32_t totFrames;
    map<int, bool> keyPressed;
    QPoint lastCursorPos;

    void processUserInput();
};
