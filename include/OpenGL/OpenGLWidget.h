#pragma once

#include <OpenGL/Common.h>
#include <OpenGL/OpenGLRenderer.h>
#include <OpenGL/FPSCounter.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget * parent = 0);
    OpenGLWidget(Scene * scene, OpenGLRenderer* renderer, QWidget * parent = 0);

    void setScene(Scene* scene);
    void setRenderer(OpenGLRenderer* renderer);
    void setRealTimeRendering(bool realTimeRendering);
    void setCaptureUserInput(bool captureUserInput);

protected:
    void childEvent(QChildEvent *event) override;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void fpsChanged(int fps);

private:
    QHash<int, bool> m_keyPressed;
    QPoint m_lastCursorPos;
    bool m_realTimeRendering, m_captureUserInput;
    OpenGLRenderer * m_renderer;
    Scene* m_host;
    FPSCounter* m_fpsCounter;

    void processUserInput();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};

