#include <OpenGLWindow.h>
#include <ModelLoader.h>

OpenGLWindow::OpenGLWindow(OpenGLRenderer* renderer) {
    m_lastCursorPos = QCursor::pos();
    m_captureUserInput = true;
    m_renderer = renderer;
    m_fpsCounter = new FPSCounter(this);
    setScene(0);
    configSignals();
}

OpenGLWindow::OpenGLWindow(OpenGLScene * openGLScene, OpenGLRenderer * renderer) {
    m_lastCursorPos = QCursor::pos();
    m_captureUserInput = true;
    m_renderer = renderer;
    m_fpsCounter = new FPSCounter(this);
    setScene(openGLScene);
    configSignals();
}

void OpenGLWindow::setScene(OpenGLScene* openGLScene) {
    m_openGLScene = openGLScene;
    if (m_openGLScene) {
        connect(m_openGLScene, SIGNAL(destroyed(QObject*)), this, SLOT(sceneDestroyed(QObject*)));
        m_openGLScene->host()->camera()->setAspectRatio(float(width()) / height());
    }
}

void OpenGLWindow::setCaptureUserInput(bool captureUserInput) {
    m_captureUserInput = captureUserInput;
}

void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    if (m_renderer) {
        if (!m_renderer->reloadShaders()) {
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
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_captureUserInput)
        processUserInput();

    if (m_openGLScene && m_openGLScene->host()->camera()) {
        m_openGLScene->host()->camera()->setAspectRatio(float(width()) / height());
        m_openGLScene->commitCameraInfo();
        m_openGLScene->commitLightInfo();

        if (!m_keyPressed[Qt::LeftButton]) {
            uint32_t pickingID = m_renderer->pickingPass(m_openGLScene, mapFromGlobal(QCursor::pos()) * devicePixelRatioF());
            OpenGLMesh* pickedOpenGLMesh = m_openGLScene->pick(pickingID);
            if (pickedOpenGLMesh)
                pickedOpenGLMesh->host()->setHighlighted(true);
            else if (Mesh::getHighlighted())
                Mesh::getHighlighted()->setHighlighted(false);
        }

        m_renderer->render(m_openGLScene);
    }
}

bool OpenGLWindow::event(QEvent * event) {
    if (QOpenGLWindow::event(event)) return true;
    if (event->type() == QEvent::DragEnter) {
        QDragEnterEvent* dragEnterEvent = static_cast<QDragEnterEvent*>(event);
        if (dragEnterEvent->mimeData()->hasUrls())
            dragEnterEvent->acceptProposedAction();
        return true;
    } else if (event->type() == QEvent::DragMove) {
        QDragMoveEvent* dragMoveEvent = static_cast<QDragMoveEvent*>(event);
        if (dragMoveEvent->mimeData()->hasUrls())
            dragMoveEvent->acceptProposedAction();
        return true;
    } else if (event->type() == QEvent::Drop) {
        if (!m_openGLScene) return true;
        QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
        foreach(const QUrl &url, dropEvent->mimeData()->urls()) {
            ModelLoader loader;
            Model* model = loader.loadModelFromFile(url.toLocalFile());
            if (model == 0) {
                QString log = loader.log();
                qWarning() << "Failed to load" << url.toLocalFile();
                qWarning() << log;
                QMessageBox::critical(0, "Failed to load file", log);
            } else
                m_openGLScene->host()->addModel(model);
        }
        return true;
    }
    return false;
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
    m_lastCursorPos = mapFromGlobal(QCursor::pos());
    m_lastMousePressTime = QTime::currentTime();
    m_keyPressed[event->button()] = true;
    if (event->button() == Qt::LeftButton && Mesh::getHighlighted()) {
        if (AbstractGizmo* gizmo = qobject_cast<AbstractGizmo*>(Mesh::getHighlighted()->parent())) {
            for (int i = 0; i < gizmo->markers().size(); i++)
                if (gizmo->markers()[i] == Mesh::getHighlighted())
                    gizmo->setTransformAxis(static_cast<AbstractGizmo::TransformAxis>(AbstractGizmo::X + i));
        }
    }
    event->accept();
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent * event) {
    m_openGLScene->host()->transformGizmo()->setTransformAxis(TransformGizmo::None);
    if (m_lastMousePressTime.msecsTo(QTime::currentTime()) < 200) { // click
        if (AbstractEntity::getHighlighted()) {
            if (AbstractGizmo* gizmo = qobject_cast<AbstractGizmo*>(Mesh::getHighlighted()->parent())) {
                gizmo->setTransformAxis(AbstractGizmo::None);
            } else {
                AbstractEntity::getHighlighted()->setSelected(true);
                m_openGLScene->host()->transformGizmo()->bindTo(AbstractEntity::getSelected());
            }
        } else if (AbstractEntity::getSelected()) {
            AbstractEntity::getSelected()->setSelected(false);
            m_openGLScene->host()->transformGizmo()->unbind();
        }
    }
    m_keyPressed[event->button()] = false;
    event->accept();
}

void OpenGLWindow::wheelEvent(QWheelEvent * event) {
    if (!m_captureUserInput || !m_openGLScene) return;

    if (!event->pixelDelta().isNull())
        m_openGLScene->host()->camera()->moveForward(event->pixelDelta().y());
    else if (!event->angleDelta().isNull())
        m_openGLScene->host()->camera()->moveForward(event->angleDelta().y());

    event->accept();
}

void OpenGLWindow::focusOutEvent(QFocusEvent *) {
    for (int i = 0; i < m_keyPressed.keys().size(); i++)
        m_keyPressed[m_keyPressed.keys()[i]] = false;
}

void OpenGLWindow::processUserInput() {
    if (!m_openGLScene || !m_openGLScene->host()->camera()) return;
    float shift = 1.0f;
    if (m_keyPressed[Qt::Key_Shift]) shift *= 5.0f;
    if (m_keyPressed[Qt::Key_W]) m_openGLScene->host()->camera()->moveForward(shift);
    if (m_keyPressed[Qt::Key_S]) m_openGLScene->host()->camera()->moveForward(-shift);
    if (m_keyPressed[Qt::Key_A]) m_openGLScene->host()->camera()->moveRight(-shift);
    if (m_keyPressed[Qt::Key_D]) m_openGLScene->host()->camera()->moveRight(shift);
    if (m_keyPressed[Qt::Key_Q]) m_openGLScene->host()->camera()->moveUp(-shift);
    if (m_keyPressed[Qt::Key_E]) m_openGLScene->host()->camera()->moveUp(shift);

    if (m_keyPressed[Qt::LeftButton]) {
        QPoint cntCursorPos = mapFromGlobal(QCursor::pos());
        TransformGizmo* gizmo = m_openGLScene->host()->transformGizmo();
        if (gizmo->visible() && gizmo->transformAxis() != TransformGizmo::None) {
            gizmo->drag(m_lastCursorPos, cntCursorPos,
                       width(), height(),
                       m_openGLScene->host()->camera()->projectionMatrix(),
                       m_openGLScene->host()->camera()->viewMatrix());
        } else {
            m_openGLScene->host()->camera()->turnLeft((m_lastCursorPos.x() - cntCursorPos.x()) / 10.0f);
            m_openGLScene->host()->camera()->lookUp((m_lastCursorPos.y() - cntCursorPos.y()) / 10.0f);
        }
        m_lastCursorPos = cntCursorPos;
    }
}

void OpenGLWindow::configSignals() {
    connect(m_fpsCounter, SIGNAL(fpsChanged(int)), this, SIGNAL(fpsChanged(int)));
    connect(this, SIGNAL(frameSwapped()), m_fpsCounter, SLOT(inc()));
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow::sceneDestroyed(QObject * host) {
    if (host == m_openGLScene)
        m_openGLScene = 0;
}
