#include <UI/MainWindow.h>
#include <Generic/Scene.h>
#include <Generic/Helper.h>
#include <IO/Loader.h>
#include <OpenGL/OpenGLManager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);

    m_menuBar = new MainMenuBar(this);
    m_centralWidget = new CentralWidget(this);
    m_statusBar = new QStatusBar(this);

    setMenuBar(m_menuBar);
    setCentralWidget(m_centralWidget);
    setStatusBar(m_statusBar);
}

MainWindow::~MainWindow() {
    delete m_menuBar;
    delete m_centralWidget;
    delete m_statusBar;
}

void MainWindow::dragEnterEvent(QDragEnterEvent * event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent * event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent * event) {
    foreach(const QUrl &url, event->mimeData()->urls()) {
        QString str = url.toLocalFile();
        Model* newModel = Loader::loadFromFile(url.toLocalFile());
        Scene::currentScene()->addModel(newModel);
        m_centralWidget->sceneTreeView->reset();
    }
}
