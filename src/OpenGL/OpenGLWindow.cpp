#include <OpenGL/OpenGLWindow.h>

OpenGLWindow::OpenGLWindow(): QOpenGLWindow(0) {
    resize(1280, 720);
    renderer = new OpenGLRenderer;
    lastCursorPos = QCursor::pos();
    time.restart();
}

OpenGLWindow::~OpenGLWindow() {
    delete renderer;
}

void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow::paintGL() {
    processUserInput();
    //renderer->render(Scene::currentScene());

    totFrames++;
    if (time.elapsed() >= 1000) { // 1000ms = 1s
        time.restart();
        qWarning() << totFrames;
        totFrames = 0;
    }
}

void OpenGLWindow::resizeGL(int width, int height) {
    //Scene::currentScene()->camera()->setAspectRatio(1.0 * width / height);
}

void OpenGLWindow::keyPressEvent(QKeyEvent * event) {
    keyPressed[event->key()] = true;
}

void OpenGLWindow::keyReleaseEvent(QKeyEvent * event) {
    keyPressed[event->key()] = false;
}

void OpenGLWindow::mousePressEvent(QMouseEvent * event) {
    lastCursorPos = QCursor::pos();
    keyPressed[event->button()] = true;
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent * event) {
    keyPressed[event->button()] = false;
}

void OpenGLWindow::processUserInput() {
    float shift = 1.0f;
    if (keyPressed[Qt::Key_Shift]) shift *= 5.0f;
    /*if (keyPressed[Qt::Key_W]) Scene::currentScene()->camera()->moveForward(shift);
    if (keyPressed[Qt::Key_S]) Scene::currentScene()->camera()->moveForward(-shift);
    if (keyPressed[Qt::Key_A]) Scene::currentScene()->camera()->moveRight(-shift);
    if (keyPressed[Qt::Key_D]) Scene::currentScene()->camera()->moveRight(shift);
    if (keyPressed[Qt::Key_Q]) Scene::currentScene()->camera()->moveUp(-shift);
    if (keyPressed[Qt::Key_E]) Scene::currentScene()->camera()->moveUp(shift);

    if (keyPressed[Qt::LeftButton]) {
        QPoint cntCursorPos = QCursor::pos();
        int dx = cntCursorPos.x() - lastCursorPos.x();
        int dy = cntCursorPos.y() - lastCursorPos.y();
        lastCursorPos = cntCursorPos;
        Scene::currentScene()->camera()->turnLeft(-dx / 10.0f);
        Scene::currentScene()->camera()->lookUp(-dy / 10.0f);
    }*/
}
