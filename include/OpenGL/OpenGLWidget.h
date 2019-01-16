#pragma once

#include <OpenGL/Common.h>
#include <OpenGL/OpenGLRenderer.h>

class FPSCounter;

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget * parent = 0);
    OpenGLWidget(Scene * scene, OpenGLRenderer* renderer, QWidget * parent = 0);

    void setScene(Scene* scene);
    void setRenderer(OpenGLRenderer* renderer);
    void setCaptureUserInput(bool captureUserInput);

    Scene* scene();
    OpenGLRenderer* renderer();
    bool captureUserInput();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void fpsChanged(int fps);

private:
    QHash<int, bool> m_keyPressed;
    QPoint m_lastCursorPos;
    bool m_captureUserInput;
    OpenGLRenderer * m_renderer;
    Scene* m_host;
    FPSCounter* m_fpsCounter;

    void processUserInput();
    void configSignals();
    void configFormat();
};

class FPSCounter: public QObject {
    Q_OBJECT

public:
    FPSCounter(QObject* parent = 0): QObject(parent) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
        timer->start(1000);
    }

public slots:
    void inc() {
        fps++;
    }

signals:
    void fpsChanged(int fps);

private:
    QTimer* timer;
    int fps;

private slots:
    void timeout() {
        fpsChanged(fps);
        fps = 0;
    }
};