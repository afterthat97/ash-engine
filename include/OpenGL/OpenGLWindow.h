#pragma once

#include <OpenGLScene.h>
#include <OpenGLRenderer.h>
#include <FPSCounter.h>

class OpenGLWindow: public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    OpenGLWindow();
    OpenGLWindow(OpenGLScene* openGLScene, OpenGLRenderer* renderer);

    QString rendererName();
    QString openGLVersion();
    QString shadingLanguageVersion();

    void setScene(OpenGLScene* openGLScene);
    void setRenderer(OpenGLRenderer* renderer);
    void setEnableMousePicking(bool enabled);
    void setCustomRenderingLoop(void (*customRenderingLoop)(Scene*));

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
    bool m_enableMousePicking;
    OpenGLScene* m_openGLScene;
    OpenGLRenderer * m_renderer;
    FPSCounter* m_fpsCounter;
    void (*m_customRenderingLoop)(Scene*);

    void processUserInput();
    void configSignals();

private slots:
    void sceneDestroyed(QObject* host);
};
