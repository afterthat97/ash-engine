#include <UI/MainWindow.h>
#include <UI/CentralWidget/OpenGLWidget.h>
#include <OpenGL/OpenGLConfig.h>
#include <Generic/Helper.h>

OpenGLWidget::OpenGLWidget(MainWindow* _mainWindow, QWidget * parent)
    : QOpenGLWidget(parent), mainWindow(_mainWindow) {
    setFocusPolicy(Qt::StrongFocus);

    totFrames = 0;
    renderer = new OpenGLRenderer(this);
    timer = new QTimer();
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
    if (time.elapsed() >= 1000) { // 1s
        mainWindow->statusBar->showMessage("FPS: " + QString::number(totFrames));
        time.restart();
        totFrames = 0;
    }
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
    float shift = 1.0f, moveSpeed = 1.0f;
    if (keyPressed[Qt::Key_Shift]) shift *= 5.0f;
    if (keyPressed[Qt::Key_W]) Scene::currentScene()->getCamera()->moveForward(moveSpeed * shift);
    if (keyPressed[Qt::Key_S]) Scene::currentScene()->getCamera()->moveForward(-moveSpeed * shift);
    if (keyPressed[Qt::Key_A]) Scene::currentScene()->getCamera()->moveRight(-moveSpeed * shift);
    if (keyPressed[Qt::Key_D]) Scene::currentScene()->getCamera()->moveRight(moveSpeed * shift);
    if (keyPressed[Qt::Key_Q]) Scene::currentScene()->getCamera()->moveUp(-moveSpeed * shift);
    if (keyPressed[Qt::Key_E]) Scene::currentScene()->getCamera()->moveUp(moveSpeed * shift);

    if (keyPressed[Qt::LeftButton]) {
        QPoint cntCursorPos = QCursor::pos();
        int dx = cntCursorPos.x() - lastCursorPos.x();
        int dy = cntCursorPos.y() - lastCursorPos.y();
        lastCursorPos = cntCursorPos;
        Scene::currentScene()->getCamera()->turnLeft(-dx / 10.0f);
        Scene::currentScene()->getCamera()->lookUp(-dy / 10.0f);
    }
}
