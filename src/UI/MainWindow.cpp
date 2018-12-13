#include <UI/MainWindow.h>
#include <UI/MeshPropertyWidget.h>
#include <UI/LightPropertyWidget.h>
#include <UI/MaterialPropertyWidget.h>
#include <UI/TexturePropertyWidget.h>
#include <Generic/Scene.h>
#include <Generic/Helper.h>
#include <IO/Loader.h>
#include <OpenGL/OpenGLManager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    menuBar = new MainMenuBar(this);
    connect(menuBar, SIGNAL(sceneChanged()), this, SLOT(resetUI()));
    propertyWidget = createEmptyPropertyWidget();

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setMenuBar(menuBar);
    setCentralWidget(createCentralWidget());
    setStatusBar(createStatusBar());
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
        sceneTreeView->reset();
    }
}

QWidget * MainWindow::createEmptyPropertyWidget() {
    QWidget * emptyPropertyWidget = new QWidget(this);
    QLabel * label = new QLabel(QStringLiteral("空属性页面"), this);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(label);

    emptyPropertyWidget->setLayout(mainLayout);
    return emptyPropertyWidget;
}

QStatusBar * MainWindow::createStatusBar() {
    return new QStatusBar(this);
}

QWidget * MainWindow::createCentralWidget() {
    QWidget * centralWidget = new QWidget(this);
    openGLWidget = new OpenGLWidget(centralWidget);
    sceneTreeView = new SceneTreeView(centralWidget);

    SceneTreeModel * sceneTreeModel = new SceneTreeModel;
    sceneTreeView->setModel(sceneTreeModel);
    connect(sceneTreeView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(reloadPropertyWidget(QModelIndex, QModelIndex)));

    splitter = new QSplitter(centralWidget);
    splitter->addWidget(sceneTreeView);
    splitter->addWidget(openGLWidget);
    splitter->addWidget(propertyWidget);
    splitter->setSizes(QList<int>{200, 500, 260});

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addWidget(splitter);
    centralWidget->setLayout(mainLayout);

    return centralWidget;
}

void MainWindow::resetUI() {
    sceneTreeView->selectionModel()->clearSelection();
    sceneTreeView->reset();
    delete propertyWidget;
    propertyWidget = createEmptyPropertyWidget();
    splitter->addWidget(propertyWidget);
    splitter->setSizes(QList<int>{200, 500, 260});
}

void MainWindow::reloadPropertyWidget(const QModelIndex & current, const QModelIndex &) {
    if (!current.isValid()) return;
    delete propertyWidget;
    Object * currentPointer = static_cast<Object*>(current.internalPointer());
    if (Model* currentItem = dynamic_cast<Model*>(currentPointer)) {
        propertyWidget = createEmptyPropertyWidget();
    } else if (Light* currentItem = dynamic_cast<Light*>(currentPointer)) {
        propertyWidget = new LightPropertyWidget(currentItem, this);
    } else if (Mesh* currentItem = dynamic_cast<Mesh*>(currentPointer)) {
        propertyWidget = new MeshPropertyWidget(currentItem, this);
    } else if (Material* currentItem = dynamic_cast<Material*>(currentPointer)) {
        propertyWidget = new MaterialPropertyWidget(currentItem, this);
    } else if (Texture* currentItem = dynamic_cast<Texture*>(currentPointer)) {
        propertyWidget = new TexturePropertyWidget(currentItem, this);
    }
    splitter->addWidget(propertyWidget);
    splitter->setSizes(QList<int>{200, 500, 260});
}
