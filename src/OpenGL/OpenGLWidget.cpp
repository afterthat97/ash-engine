#include <OpenGL/OpenGLWidget.h>
#include <IO/ModelLoader.h>

OpenGLWidget::OpenGLWidget(QWidget * parent): QOpenGLWidget(parent) {
    m_lastCursorPos = QCursor::pos();
    m_realTimeRendering = true;
    m_captureUserInput = true;
    m_renderer = 0;
    m_host = 0;
    m_fpsCounter = new FPSCounter(this);

    configSignals();
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
}

OpenGLWidget::OpenGLWidget(Scene * scene, OpenGLRenderer* renderer, QWidget * parent): QOpenGLWidget(parent) {
    m_lastCursorPos = QCursor::pos();
    m_realTimeRendering = true;
    m_captureUserInput = true;
    m_renderer = renderer;
    m_host = scene;
    m_fpsCounter = new FPSCounter(this);

    configSignals();
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
}

void OpenGLWidget::setRenderer(OpenGLRenderer * renderer) {
    m_renderer = renderer;
}

void OpenGLWidget::setRealTimeRendering(bool realTimeRendering) {
    m_realTimeRendering = realTimeRendering;
}

void OpenGLWidget::setScene(Scene * scene) {
    m_host = scene;
}

void OpenGLWidget::setCaptureUserInput(bool captureUserInput) {
    m_captureUserInput = captureUserInput;
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::paintGL() {
    if (!m_realTimeRendering) return;
    if (m_captureUserInput)
        processUserInput();
    if (m_host) m_renderer->render(m_host);
}

void OpenGLWidget::resizeGL(int width, int height) {
    if (m_host) m_host->camera()->setAspectRatio(1.0 * width / height);
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event) {
    m_keyPressed[event->key()] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent * event) {
    m_keyPressed[event->key()] = false;
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event) {
    m_lastCursorPos = QCursor::pos();
    m_keyPressed[event->button()] = true;
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event) {
    m_keyPressed[event->button()] = false;
}

void OpenGLWidget::dragEnterEvent(QDragEnterEvent * event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void OpenGLWidget::dragMoveEvent(QDragMoveEvent * event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void OpenGLWidget::dropEvent(QDropEvent * event) {
    if (!m_host) return;
    foreach(const QUrl &url, event->mimeData()->urls()) {
        ModelLoader loader;
        m_host->addModel(loader.loadFromFile(url.toLocalFile()));
    }
}

void OpenGLWidget::processUserInput() {
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

void OpenGLWidget::configSignals() {
    connect(m_fpsCounter, SIGNAL(fpsChanged(int)), this, SIGNAL(fpsChanged(int)));
    connect(this, SIGNAL(frameSwapped()), m_fpsCounter, SLOT(inc()));
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWidget::hostDestroyed(QObject * host) {
    if (host == m_host)
        m_host = 0;
}
