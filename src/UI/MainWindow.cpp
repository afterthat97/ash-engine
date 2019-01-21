#include <UI/MainWindow.h>
#include <UI/CameraProperty.h>
#include <UI/GridlineProperty.h>
#include <UI/AmbientLightProperty.h>
#include <UI/DirectionalLightProperty.h>
#include <UI/PointLightProperty.h>
#include <UI/SpotLightProperty.h>
#include <UI/ModelProperty.h>
#include <UI/MeshProperty.h>
#include <UI/MaterialProperty.h>

MainWindow::MainWindow(Scene * scene, QWidget * parent): QMainWindow(parent) {
    m_host = scene;

    m_menuBar = new QMenuBar(this);
    m_centralWidget = new QWidget(this);
    m_splitter = new QSplitter(this);
    m_statusBar = new QStatusBar(this);
    m_fpsLabel = new QLabel(this);
    m_statusBar->addPermanentWidget(m_fpsLabel);

    m_sceneTreeWidget = new SceneTreeWidget(m_host, this);
    m_openGLWindow = new OpenGLWindow(m_host, new OpenGLRenderer);
    m_propertyWidget = new QWidget(this);

    setMenuBar(m_menuBar);
    setCentralWidget(m_centralWidget);
    setStatusBar(m_statusBar);
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);

    configMenu();
    configLayout();
    configSignals();

    resize(1200, 720);
    helpCheckForUpdates();
}

void MainWindow::resizeEvent(QResizeEvent *) {
    m_splitter->setSizes(QList<int>{160, width() - 160 - 300, 300});
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
    if (!m_host) return;
    foreach(const QUrl &url, event->mimeData()->urls()) {
        ModelLoader loader;
        m_host->addModel(loader.loadFromFile(url.toLocalFile()));
    }
}

void MainWindow::configMenu() {
    actionFileNew = new QAction("New", this);
    actionFileOpen = new QAction("Open", this);
    actionFileLoadModel = new QAction("Load Model", this);
    actionFileSave = new QAction("Save", this);
    actionFileExit = new QAction("Exit", this);

    actionCreateGridline = new QAction("Gridline", this);

    actionCreateAmbientLight = new QAction("Ambient Light", this);
    actionCreateDirectionalLight = new QAction("Directional Light", this);
    actionCreatePointLight = new QAction("Point Light", this);
    actionCreateSpotLight = new QAction("Spot Light", this);

    actionCreateBasicCone = new QAction("Cone", this);
    actionCreateBasicCube = new QAction("Cube", this);
    actionCreateBasicCylinder = new QAction("Cylinder", this);
    actionCreateBasicPlane = new QAction("Plane", this);
    actionCreateBasicSphere = new QAction("Sphere", this);

    actionHelpAbout = new QAction("About", this);
    actionHelpCheckUpdate = new QAction("Check for Update", this);

    QMenu *menuFile = m_menuBar->addMenu("File");
    menuFile->addAction(actionFileNew);
    menuFile->addAction(actionFileOpen);
    menuFile->addAction(actionFileLoadModel);
    menuFile->addAction(actionFileSave);
    menuFile->addAction(actionFileExit);

    QMenu *menuCreate = m_menuBar->addMenu("Create");
    menuCreate->addAction(actionCreateGridline);

    QMenu *menuCreateLight = menuCreate->addMenu("Light");
    menuCreateLight->addAction(actionCreateAmbientLight);
    menuCreateLight->addAction(actionCreateDirectionalLight);
    menuCreateLight->addAction(actionCreatePointLight);
    menuCreateLight->addAction(actionCreateSpotLight);

    QMenu *menuCreateBasicShapes = menuCreate->addMenu("Basic Shape");
    menuCreateBasicShapes->addAction(actionCreateBasicCone);
    menuCreateBasicShapes->addAction(actionCreateBasicCube);
    menuCreateBasicShapes->addAction(actionCreateBasicCylinder);
    menuCreateBasicShapes->addAction(actionCreateBasicPlane);
    menuCreateBasicShapes->addAction(actionCreateBasicSphere);

    QMenu *menuHelp = m_menuBar->addMenu("Help");
    menuHelp->addAction(actionHelpAbout);
    menuHelp->addAction(actionHelpCheckUpdate);
}

void MainWindow::configLayout() {
    m_splitter->addWidget(m_sceneTreeWidget);
    m_splitter->addWidget(QWidget::createWindowContainer(m_openGLWindow));
    m_splitter->addWidget(m_propertyWidget);
    m_splitter->setSizes(QList<int>{160, width() - 160 - 300, 300});

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_splitter);
    m_centralWidget->setLayout(mainLayout);
}

void MainWindow::configSignals() {
    connect(m_openGLWindow, SIGNAL(fpsChanged(int)), this, SLOT(fpsChanged(int)));
    connect(m_sceneTreeWidget, SIGNAL(itemSelected(QVariant)), this, SLOT(itemSelected(QVariant)));
    connect(m_sceneTreeWidget, SIGNAL(itemDeselected(QVariant)), this, SLOT(itemDeselected(QVariant)));

    connect(actionFileNew, SIGNAL(triggered(bool)), this, SLOT(fileNew()));
    connect(actionFileOpen, SIGNAL(triggered(bool)), this, SLOT(fileOpen()));
    connect(actionFileLoadModel, SIGNAL(triggered(bool)), this, SLOT(fileLoadModel()));
    connect(actionFileSave, SIGNAL(triggered(bool)), this, SLOT(fileSave()));
    connect(actionFileExit, SIGNAL(triggered(bool)), this, SLOT(fileExit()));

    connect(actionCreateGridline, SIGNAL(triggered(bool)), this, SLOT(createGridline()));

    connect(actionCreateAmbientLight, SIGNAL(triggered(bool)), this, SLOT(createAmbientLight()));
    connect(actionCreateDirectionalLight, SIGNAL(triggered(bool)), this, SLOT(createDirectionalLight()));
    connect(actionCreatePointLight, SIGNAL(triggered(bool)), this, SLOT(createPointLight()));
    connect(actionCreateSpotLight, SIGNAL(triggered(bool)), this, SLOT(createSpotLight()));

    connect(actionCreateBasicCone, SIGNAL(triggered(bool)), this, SLOT(createBasicCone()));
    connect(actionCreateBasicCube, SIGNAL(triggered(bool)), this, SLOT(createBasicCube()));
    connect(actionCreateBasicCylinder, SIGNAL(triggered(bool)), this, SLOT(createBasicCylinder()));
    connect(actionCreateBasicPlane, SIGNAL(triggered(bool)), this, SLOT(createBasicPlane()));
    connect(actionCreateBasicSphere, SIGNAL(triggered(bool)), this, SLOT(createBasicSphere()));

    connect(actionHelpAbout, SIGNAL(triggered(bool)), this, SLOT(helpAbout()));
    connect(actionHelpCheckUpdate, SIGNAL(triggered(bool)), this, SLOT(helpCheckForUpdates()));
}

void MainWindow::fpsChanged(int fps) {
    m_fpsLabel->setText("FPS: " + QString::number(fps));
}

void MainWindow::itemSelected(QVariant item) {
    if (item.canConvert<Camera*>()) {
        m_propertyWidget = new CameraProperty(item.value<Camera*>(), this);
    } else if (item.canConvert<Gridline*>()) {
        m_propertyWidget = new GridlineProperty(item.value<Gridline*>(), this);
    } else if (item.canConvert<AmbientLight*>()) {
        m_propertyWidget = new AmbientLightProperty(item.value<AmbientLight*>(), this);
    } else if (item.canConvert<DirectionalLight*>()) {
        m_propertyWidget = new DirectionalLightProperty(item.value<DirectionalLight*>(), this);
    } else if (item.canConvert<PointLight*>()) {
        m_propertyWidget = new PointLightProperty(item.value<PointLight*>(), this);
    } else if (item.canConvert<SpotLight*>()) {
        m_propertyWidget = new SpotLightProperty(item.value<SpotLight*>(), this);
    } else if (item.canConvert<Model*>()) {
        m_propertyWidget = new ModelProperty(item.value<Model*>(), this);
    } else if (item.canConvert<Mesh*>()) {
        m_propertyWidget = new MeshProperty(item.value<Mesh*>(), this);
    } else if (item.canConvert<Material*>()) {
        m_propertyWidget = new MaterialProperty(item.value<Material*>(), this);
    }
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::itemDeselected(QVariant) {
    m_propertyWidget = new QWidget(this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::fileNew() {
    m_propertyWidget = new QWidget(this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
    delete m_host;
    m_host = new Scene;
    m_host->addGridline(new Gridline);
    m_sceneTreeWidget->setScene(m_host);
    m_openGLWindow->setScene(m_host);
}

void MainWindow::fileOpen() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Project", "", "Ash Engine Project (*.aeproj)");
    SceneLoader loader;
    Scene* scene = loader.loadFromFile(filePath);
    if (scene) {
        m_propertyWidget = new QWidget(this);
        delete m_splitter->replaceWidget(2, m_propertyWidget);
        delete m_host;
        m_host = scene;
        m_sceneTreeWidget->setScene(m_host);
        m_openGLWindow->setScene(m_host);
    }
}

void MainWindow::fileLoadModel() {
    QString filePath = QFileDialog::getOpenFileName(this, "Load Model", "", "All Files (*)");
    ModelLoader loader;
    Model* loadedModel = loader.loadFromFile(filePath);
    m_host->addModel(loadedModel);
}

void MainWindow::fileSave() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save Project", "", "Ash Engine Project (*.aeproj)");
    SceneSaver saver(m_host);
    saver.saveToFile(filePath);
}

void MainWindow::fileExit() {
    QApplication::quit();
}

void MainWindow::createGridline() {
    m_host->addGridline(new Gridline);
}

void MainWindow::createAmbientLight() {
    m_host->addLight(new AmbientLight);
}

void MainWindow::createDirectionalLight() {
    m_host->addLight(new DirectionalLight);
}

void MainWindow::createPointLight() {
    m_host->addLight(new PointLight);
}

void MainWindow::createSpotLight() {
    m_host->addLight(new SpotLight);
}

void MainWindow::createBasicCone() {
    m_host->addModel(ModelLoader::loadConeModel());
}

void MainWindow::createBasicCube() {
    m_host->addModel(ModelLoader::loadCubeModel());
}

void MainWindow::createBasicCylinder() {
    m_host->addModel(ModelLoader::loadCylinderModel());
}

void MainWindow::createBasicPlane() {
    m_host->addModel(ModelLoader::loadPlaneModel());
}

void MainWindow::createBasicSphere() {
    m_host->addModel(ModelLoader::loadSphereModel());
}

void MainWindow::helpAbout() {
    QString info = "Current version: " + QString(APP_VERSION) + "\n\n";
    info += "masterEngine is a cross-platform 3D engine for learning purpose, based on Qt, OpenGL and Assimp.\n\n";
    info += "Author: Alfred Liu\n";
    info += "Email:  afterthat97@foxmail.com";
    QMessageBox::about(this, "About", info);
}

void MainWindow::helpCheckForUpdates() {
    QString url = "https://api.github.com/repos/afterthat97/masterEngine/releases/latest";
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyOfUpdates(QNetworkReply*)));
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::replyOfUpdates(QNetworkReply * reply) {
    QString strReply = (QString) reply->readAll();
    QJsonObject jsonObject = QJsonDocument::fromJson(strReply.toUtf8()).object();

    QString latestVersion = jsonObject["tag_name"].toString();
    if (latestVersion != APP_VERSION && latestVersion != "") {
        QString info = "A new version has been released, do you want to upgrade?\n\n";
        info += "Current version: " + QString(APP_VERSION) + "\n";
        info += "Latest version: " + latestVersion;
        if (QMessageBox::question(this, "Update", info, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl(jsonObject["html_url"].toString()));
        }
    }
}
