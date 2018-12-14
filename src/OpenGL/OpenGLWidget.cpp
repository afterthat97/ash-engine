#include <OpenGL/OpenGLWidget.h>
#include <OpenGL/OpenGLConfig.h>
#include <Generic/Helper.h>

OpenGLWidget::OpenGLWidget(QWidget* parent): QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    renderer = new OpenGLRenderer(this);
    lastCursorPos = QCursor::pos();
    timer.restart();
    totFrames = 0;
}

OpenGLWidget::~OpenGLWidget() {
    delete renderer;
}

QMatrix4x4 OpenGLWidget::getProjectionMatrix() {
    QMatrix4x4 proj;
    proj.setToIdentity();
    proj.perspective(45.0f, GLfloat(size().width()) / size().height(), 0.1f, 100000.0f);
    return proj;
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    renderer->loadShader(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");
}

void OpenGLWidget::paintGL() {
    totFrames++;
    if (timer.elapsed() >= 1000) {
        cout << totFrames << endl;
        timer.restart();
        totFrames = 0;
    }
    processInput();
    renderer->render(Scene::currentScene());
    update();
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

void OpenGLWidget::processInput() {
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
