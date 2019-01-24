#include <OpenGL/OpenGLWindow.h>
#include <OpenGL/OpenGLConfig.h>

OpenGLWindow::OpenGLWindow(OpenGLRenderer* renderer) {
    m_lastCursorPos = QCursor::pos();
    m_captureUserInput = true;
    m_renderer = renderer;
    m_fpsCounter = new FPSCounter(this);
    setScene(0);
    configSignals();
}

OpenGLWindow::OpenGLWindow(Scene * scene, OpenGLRenderer * renderer) {
    m_lastCursorPos = QCursor::pos();
    m_captureUserInput = true;
    m_renderer = renderer;
    m_fpsCounter = new FPSCounter(this);
    setScene(scene);
    configSignals();
}

void OpenGLWindow::setScene(Scene * scene) {
    m_host = scene;
    if (m_host)
        connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

void OpenGLWindow::setCaptureUserInput(bool captureUserInput) {
    m_captureUserInput = captureUserInput;
}

void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    if (m_renderer) {
        if (!m_renderer->loadShaders()) {
            QString log = m_renderer->log();
            qWarning() << "OpenGLWindow::initializeGL(): Failed to initialize OpenGL: Error when loading shaders.";
            qWarning() << log;
            QMessageBox::critical(0, "Failed to load shaders", log);
        }
    } else {
        qWarning() << "OpenGLWindow::initializeGL(): Failed to initialize OpenGL: Renderer not specified.";
    }
}

void OpenGLWindow::paintGL() {
    if (m_captureUserInput)
        processUserInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(OpenGLConfig::getBackgroundColor()[0], OpenGLConfig::getBackgroundColor()[1], OpenGLConfig::getBackgroundColor()[2], 1.0f);

    if (m_host) {
        m_host->camera()->setAspectRatio(float(width()) / height());
        m_renderer->render(m_host);
    }
}

void OpenGLWindow::keyPressEvent(QKeyEvent * event) {
    m_keyPressed[event->key()] = true;
    event->accept();
}

void OpenGLWindow::keyReleaseEvent(QKeyEvent * event) {
    m_keyPressed[event->key()] = false;
    event->accept();
}

void OpenGLWindow::mousePressEvent(QMouseEvent * event) {
    m_lastCursorPos = QCursor::pos();
    m_keyPressed[event->button()] = true;
    event->accept();
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent * event) {
    m_keyPressed[event->button()] = false;
    event->accept();
}

void OpenGLWindow::wheelEvent(QWheelEvent * event) {
    if (!m_captureUserInput || !m_host) return;

    if (!event->pixelDelta().isNull())
        m_host->camera()->moveForward(event->pixelDelta().y());
    else if (!event->angleDelta().isNull())
        m_host->camera()->moveForward(event->angleDelta().y());

    event->accept();
}

void OpenGLWindow::focusOutEvent(QFocusEvent *) {
    for (int i = 0; i < m_keyPressed.keys().size(); i++)
        m_keyPressed[m_keyPressed.keys()[i]] = false;
}

void OpenGLWindow::processUserInput() {
    if (!m_host) return;
    float shift = 1.0f;
    if (m_keyPressed[Qt::Key_Shift]) shift *= 5.0f;
    if (m_keyPressed[Qt::Key_W]) m_host->camera()->moveForward(shift);
    if (m_keyPressed[Qt::Key_S]) m_host->camera()->moveForward(-shift);
    if (m_keyPressed[Qt::Key_A]) m_host->camera()->moveRight(-shift);
    if (m_keyPressed[Qt::Key_D]) m_host->camera()->moveRight(shift);
    if (m_keyPressed[Qt::Key_Q]) m_host->camera()->moveUp(-shift);
    if (m_keyPressed[Qt::Key_E]) m_host->camera()->moveUp(shift);

    if (m_keyPressed[Qt::LeftButton]) {
        QPoint cntCursorPos = QCursor::pos();
        int dx = cntCursorPos.x() - m_lastCursorPos.x();
        int dy = cntCursorPos.y() - m_lastCursorPos.y();
        m_lastCursorPos = cntCursorPos;
        m_host->camera()->turnLeft(-dx / 10.0f);
        m_host->camera()->lookUp(-dy / 10.0f);
    }
}

void OpenGLWindow::configSignals() {
    connect(m_fpsCounter, SIGNAL(fpsChanged(int)), this, SIGNAL(fpsChanged(int)));
    connect(this, SIGNAL(frameSwapped()), m_fpsCounter, SLOT(inc()));
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow::hostDestroyed(QObject * host) {
    if (host == m_host)
        m_host = 0;
}
