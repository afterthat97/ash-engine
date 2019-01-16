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

    m_sceneTreeView = new SceneTreeView(m_host, this);
    m_openGLWidget = new OpenGLWidget(m_host, new OpenGLRenderer, this);
    m_propertyWidget = new QWidget(this);

    setMenuBar(m_menuBar);
    setCentralWidget(m_centralWidget);
    setStatusBar(m_statusBar);
    configMenu();
    configLayout();
    configSignals();
}

void MainWindow::configMenu() {
    actionFileNew = new QAction("New", this);
    actionFileOpen = new QAction("Open", this);
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
    m_splitter->addWidget(m_sceneTreeView);
    m_splitter->addWidget(m_openGLWidget);
    m_splitter->addWidget(m_propertyWidget);
    m_splitter->setSizes(QList<int>{160, 500, 340});

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_splitter);
    m_centralWidget->setLayout(mainLayout);
}

void MainWindow::configSignals() {
    connect(m_openGLWidget, SIGNAL(fpsChanged(int)), this, SLOT(fpsChanged(int)));

    connect(m_sceneTreeView, SIGNAL(cameraSelected(Camera*)), this, SLOT(cameraSelected(Camera*)));
    connect(m_sceneTreeView, SIGNAL(gridlineSelected(Gridline*)), this, SLOT(gridlineSelected(Gridline*)));
    connect(m_sceneTreeView, SIGNAL(ambientLightSelected(AmbientLight*)), this, SLOT(ambientLightSelected(AmbientLight*)));
    connect(m_sceneTreeView, SIGNAL(directionalLightSelected(DirectionalLight*)), this, SLOT(directionalLightSelected(DirectionalLight*)));
    connect(m_sceneTreeView, SIGNAL(pointLightSelected(PointLight*)), this, SLOT(pointLightSelected(PointLight*)));
    connect(m_sceneTreeView, SIGNAL(spotLightSelected(SpotLight*)), this, SLOT(spotLightSelected(SpotLight*)));
    connect(m_sceneTreeView, SIGNAL(modelSelected(Model*)), this, SLOT(modelSelected(Model*)));
    connect(m_sceneTreeView, SIGNAL(meshSelected(Mesh*)), this, SLOT(meshSelected(Mesh*)));
    connect(m_sceneTreeView, SIGNAL(materialSelected(Material*)), this, SLOT(materialSelected(Material*)));

    connect(actionFileNew, SIGNAL(triggered(bool)), this, SLOT(fileNew()));
    connect(actionFileOpen, SIGNAL(triggered(bool)), this, SLOT(fileOpen()));
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
    m_statusBar->showMessage("FPS: " + QString::number(fps));
}

void MainWindow::cameraSelected(Camera * camera) {
    m_propertyWidget = new CameraProperty(camera, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::gridlineSelected(Gridline * gridline) {
    m_propertyWidget = new GridlineProperty(gridline, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::ambientLightSelected(AmbientLight * light) {
    m_propertyWidget = new AmbientLightProperty(light, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::directionalLightSelected(DirectionalLight * light) {
    m_propertyWidget = new DirectionalLightProperty(light, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::pointLightSelected(PointLight * light) {
    m_propertyWidget = new PointLightProperty(light, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::spotLightSelected(SpotLight * light) {
    m_propertyWidget = new SpotLightProperty(light, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::modelSelected(Model * model) {
    m_propertyWidget = new ModelProperty(model, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::meshSelected(Mesh * mesh) {
    m_propertyWidget = new MeshProperty(mesh, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::materialSelected(Material * material) {
    m_propertyWidget = new MaterialProperty(material, this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::fileNew() {
    m_sceneTreeView->selectionModel()->clearSelection();
    delete m_host;
    m_host = new Scene;
    m_sceneTreeView->reset();
    m_propertyWidget = new CameraProperty(m_host->camera(), this);
    delete m_splitter->replaceWidget(2, m_propertyWidget);
}

void MainWindow::fileOpen() {
    QString filepath = QFileDialog::getOpenFileName(this, "Load Model(s)", "", "All Files (*)");
    ModelLoader loader;
    Model* loadedModel = loader.loadFromFile(filepath);
    m_host->addModel(loadedModel);
    //m_host->camera()->setDirection(loadedModel->centerOfMass() - m_host->camera()->position());
    m_sceneTreeView->reset();
}

void MainWindow::fileExit() {
    QApplication::quit();
}

void MainWindow::createGridline() {
    m_host->addGridline(new Gridline);
    m_sceneTreeView->reset();
}

void MainWindow::createAmbientLight() {
    m_host->addLight(new AmbientLight);
    m_sceneTreeView->reset();
}

void MainWindow::createDirectionalLight() {
    m_host->addLight(new DirectionalLight);
    m_sceneTreeView->reset();
}

void MainWindow::createPointLight() {
    m_host->addLight(new PointLight);
    m_sceneTreeView->reset();
}

void MainWindow::createSpotLight() {
    m_host->addLight(new SpotLight);
    m_sceneTreeView->reset();
}

void MainWindow::createBasicCone() {
    m_host->addModel(ModelLoader::loadConeModel());
    m_sceneTreeView->reset();
}

void MainWindow::createBasicCube() {
    m_host->addModel(ModelLoader::loadCubeModel());
    m_sceneTreeView->reset();
}

void MainWindow::createBasicCylinder() {
    m_host->addModel(ModelLoader::loadCylinderModel());
    m_sceneTreeView->reset();
}

void MainWindow::createBasicPlane() {
    m_host->addModel(ModelLoader::loadPlaneModel());
    m_sceneTreeView->reset();
}

void MainWindow::createBasicSphere() {
    m_host->addModel(ModelLoader::loadSphereModel());
    m_sceneTreeView->reset();
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

