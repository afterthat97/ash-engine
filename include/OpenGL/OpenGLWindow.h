#pragma once

#include <OpenGLScene.h>
#include <OpenGLRenderer.h>
#include <FPSCounter.h>

class OpenGLWindow: public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    OpenGLWindow(OpenGLRenderer* renderer);
    OpenGLWindow(OpenGLScene* openGLScene, OpenGLRenderer* renderer);

    void setScene(OpenGLScene* openGLScene);
    void setCaptureUserInput(bool captureUserInput);

protected:
    void initializeGL() override;
    void paintGL() override;
    bool event(QEvent *ev) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void fpsChanged(int fps);

private:
    QHash<int, bool> m_keyPressed;
    QPoint m_lastCursorPos;
    QTime m_lastMousePressTime;
    bool m_captureUserInput;
    OpenGLScene* m_openGLScene;
    OpenGLRenderer * m_renderer;
    FPSCounter* m_fpsCounter;

    void processUserInput();
    void configSignals();

private slots:
    void sceneDestroyed(QObject* host);
};
