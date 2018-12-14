#include <UI/MainWindow.h>
#include <Generic/Scene.h>
#include <Generic/Helper.h>
#include <IO/Loader.h>
#include <OpenGL/OpenGLManager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);

    menuBar = new MainMenuBar(this);
    centralWidget = new CentralWidget(this);
    statusBar = new QStatusBar(this);

    setMenuBar(menuBar);
    setCentralWidget(centralWidget);
    setStatusBar(statusBar);

    connect(centralWidget->sceneTreeView, SIGNAL(modelSelected(Model*, bool)), this, SLOT(modelSelected(Model*, bool)));
    connect(centralWidget->sceneTreeView, SIGNAL(lightSelected(Light*, bool)), this, SLOT(lightSelected(Light*, bool)));
    connect(centralWidget->sceneTreeView, SIGNAL(meshSelected(Mesh*, bool)), this, SLOT(meshSelected(Mesh*, bool)));
    connect(centralWidget->sceneTreeView, SIGNAL(materialSelected(Material*, bool)), this, SLOT(materialSelected(Material*, bool)));
    connect(centralWidget->sceneTreeView, SIGNAL(textureSelected(Texture*, bool)), this, SLOT(textureSelected(Texture*, bool)));
}

MainWindow::~MainWindow() {
    delete menuBar;
    delete centralWidget;
    delete statusBar;
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
        centralWidget->sceneTreeView->reset();
    }
}

void MainWindow::modelSelected(Model* model, bool selected) {
    if (selected) {
        centralWidget->setPropertyWidget(model);
    }
}

void MainWindow::lightSelected(Light * light, bool selected) {
    if (selected) {
        centralWidget->setPropertyWidget(light);
    }
}

void MainWindow::meshSelected(Mesh * mesh, bool selected) {
    if (selected) {
        centralWidget->setPropertyWidget(mesh);
    }
}

void MainWindow::materialSelected(Material * material, bool selected) {
    if (selected) {
        centralWidget->setPropertyWidget(material);
    }
}

void MainWindow::textureSelected(Texture * texture, bool selected) {
    if (selected) {
        centralWidget->setPropertyWidget(texture);
    }
}
