#include <UI/MainWindow.h>
#include <UI/CentralWidget/OpenGLWidget.h>
#include <OpenGL/OpenGLConfig.h>
#include <Generic/Helper.h>

OpenGLWidget::OpenGLWidget(QWidget * parent): QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);

    renderer = new OpenGLRenderer;
    timer = new QTimer;
    
    totFrames = 0;
    lastCursorPos = QCursor::pos();
}

OpenGLWidget::~OpenGLWidget() {
    delete renderer;
    delete timer;
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    renderer->loadShader(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");
    glEnable(GL_DEPTH_TEST);

    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
    timer->start();
    time.restart();
}

void OpenGLWidget::paintGL() {
    processUserInput();
    renderer->render(Scene::currentScene());

    totFrames++;
    if (time.elapsed() >= 1000) { // 1000ms = 1s
        FPSChanged(totFrames);
        time.restart();
        totFrames = 0;
    }
}

void OpenGLWidget::resizeGL(int width, int height) {
    Scene::currentScene()->getCamera()->setAspectRatio(1.0 * width / height);
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event) {
    keyPressed[event->key()] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent * event) {
    keyPressed[event->key()] = false;
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event) {
    lastCursorPos = QCursor::pos();
    keyPressed[event->button()] = true;
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event) {
    keyPressed[event->button()] = false;
}

void OpenGLWidget::processUserInput() {
    float shift = 1.0f;
    if (keyPressed[Qt::Key_Shift]) shift *= 5.0f;
    if (keyPressed[Qt::Key_W]) Scene::currentScene()->getCamera()->moveForward(shift);
    if (keyPressed[Qt::Key_S]) Scene::currentScene()->getCamera()->moveForward(-shift);
    if (keyPressed[Qt::Key_A]) Scene::currentScene()->getCamera()->moveRight(-shift);
    if (keyPressed[Qt::Key_D]) Scene::currentScene()->getCamera()->moveRight(shift);
    if (keyPressed[Qt::Key_Q]) Scene::currentScene()->getCamera()->moveUp(-shift);
    if (keyPressed[Qt::Key_E]) Scene::currentScene()->getCamera()->moveUp(shift);

    if (keyPressed[Qt::LeftButton]) {
        QPoint cntCursorPos = QCursor::pos();
        int dx = cntCursorPos.x() - lastCursorPos.x();
        int dy = cntCursorPos.y() - lastCursorPos.y();
        lastCursorPos = cntCursorPos;
        Scene::currentScene()->getCamera()->turnLeft(-dx / 10.0f);
        Scene::currentScene()->getCamera()->lookUp(-dy / 10.0f);
    }
}
