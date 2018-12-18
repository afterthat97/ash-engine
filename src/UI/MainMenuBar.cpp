#include <UI/MainMenuBar.h>
#include <UI/MainWindow.h>
#include <Generic/Scene.h>
#include <Generic/Helper.h>
#include <IO/Loader.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLConfig.h>

MainMenuBar::MainMenuBar(MainWindow *parent): QMenuBar(parent), mainWindow(parent) {
    createActions();
    createMenus();
    helpCheckForUpdates();
}

MainMenuBar::~MainMenuBar() {
    delete menuFile;
    delete menuCreate;
    delete menuOptions;
    delete menuHelp;

    delete actionFileNew;
    delete actionFileOpen;
    delete actionFileExit;

    delete actionCreateLight;
    delete actionCreateBasicCone;
    delete actionCreateBasicCube;
    delete actionCreateBasicCylinder;
    delete actionCreateBasicPlane;
    delete actionCreateBasicSphere;

    delete actionEnableLighting;
    delete actionEnableGridline;
    delete actionEnableWireFrame;

    delete actionSetFPS30;
    delete actionSetFPS60;

    delete actionHelpAbout;
    delete actionHelpCheckUpdate;
}

void MainMenuBar::createActions() {
    actionFileNew = new QAction("New", this);
    actionFileOpen = new QAction("Open", this);
    actionFileExit = new QAction("Exit", this);

    actionCreateLight = new QAction("Light", this);
    actionCreateBasicCone = new QAction("Cone", this);
    actionCreateBasicCube = new QAction("Cube", this);
    actionCreateBasicCylinder = new QAction("Cylinder", this);
    actionCreateBasicPlane = new QAction("Plane", this);
    actionCreateBasicSphere = new QAction("Sphere", this);

    actionEnableLighting = new QAction("Enable Lighting", this);
    actionEnableGridline = new QAction("Enable Gridline", this);
    actionEnableWireFrame = new QAction("WireFrame Mode", this);
    actionEnableLighting->setCheckable(true);
    actionEnableGridline->setCheckable(true);
    actionEnableWireFrame->setCheckable(true);
    actionEnableLighting->setChecked(OpenGLConfig::isLightingEnabled());
    actionEnableGridline->setChecked(OpenGLConfig::isGridlineEnabled());
    actionEnableWireFrame->setChecked(OpenGLConfig::isWireFrameEnabled());

    actionSetFPS30 = new QAction("30", this);
    actionSetFPS60 = new QAction("60", this);
    actionSetFPS30->setCheckable(true);
    actionSetFPS60->setCheckable(true);
    actionSetFPS30->setChecked(false);
    actionSetFPS60->setChecked(true);

    actionHelpAbout = new QAction("About", this);
    actionHelpCheckUpdate = new QAction("Check for Update", this);

    connect(actionFileNew, SIGNAL(triggered(bool)), this, SLOT(fileNew()));
    connect(actionFileOpen, SIGNAL(triggered(bool)), this, SLOT(fileOpen()));
    connect(actionFileExit, SIGNAL(triggered(bool)), this, SLOT(fileExit()));

    connect(actionCreateLight, SIGNAL(triggered(bool)), this, SLOT(createLight()));
    connect(actionCreateBasicCone, SIGNAL(triggered(bool)), this, SLOT(createBasicCone()));
    connect(actionCreateBasicCube, SIGNAL(triggered(bool)), this, SLOT(createBasicCube()));
    connect(actionCreateBasicCylinder, SIGNAL(triggered(bool)), this, SLOT(createBasicCylinder()));
    connect(actionCreateBasicPlane, SIGNAL(triggered(bool)), this, SLOT(createBasicPlane()));
    connect(actionCreateBasicSphere, SIGNAL(triggered(bool)), this, SLOT(createBasicSphere()));
    
    connect(actionEnableLighting, SIGNAL(triggered(bool)), this, SLOT(optionLighting(bool)));
    connect(actionEnableGridline, SIGNAL(triggered(bool)), this, SLOT(optionGridline(bool)));
    connect(actionEnableWireFrame, SIGNAL(triggered(bool)), this, SLOT(optionWireFrame(bool)));

    connect(actionSetFPS30, SIGNAL(triggered(bool)), this, SLOT(setFPS30()));
    connect(actionSetFPS60, SIGNAL(triggered(bool)), this, SLOT(setFPS60()));

    connect(actionHelpAbout, SIGNAL(triggered(bool)), this, SLOT(helpAbout()));
    connect(actionHelpCheckUpdate, SIGNAL(triggered(bool)), this, SLOT(helpCheckForUpdates()));
}

void MainMenuBar::createMenus() {
    menuFile = this->addMenu("File");
    menuFile->addAction(actionFileNew);
    menuFile->addAction(actionFileOpen);
    menuFile->addAction(actionFileExit);

    menuCreate = this->addMenu("Create");
    menuCreate->addAction(actionCreateLight);

    menuCreateBasicShapes = menuCreate->addMenu("Basic Shapes");
    menuCreateBasicShapes->addAction(actionCreateBasicCone);
    menuCreateBasicShapes->addAction(actionCreateBasicCube);
    menuCreateBasicShapes->addAction(actionCreateBasicCylinder);
    menuCreateBasicShapes->addAction(actionCreateBasicPlane);
    menuCreateBasicShapes->addAction(actionCreateBasicSphere);

    menuOptions = this->addMenu("Options");
    menuOptions->addAction(actionEnableLighting);
    menuOptions->addAction(actionEnableGridline);
    menuOptions->addAction(actionEnableWireFrame);

    menuSetFPS = menuOptions->addMenu("Set FPS");
    menuSetFPS->addAction(actionSetFPS30);
    menuSetFPS->addAction(actionSetFPS60);

    menuHelp = this->addMenu("Help");
    menuHelp->addAction(actionHelpAbout);
    menuHelp->addAction(actionHelpCheckUpdate);
}

void MainMenuBar::fileNew() {
    mainWindow->m_centralWidget->sceneTreeView->selectionModel()->clearSelection();
    Allocator::resetLightIDCounter();
    Scene::currentScene()->clean();
    OpenGLManager::cleanOpenGLMesh();
    OpenGLManager::cleanOpenGLTexture();
    mainWindow->m_centralWidget->sceneTreeView->reset();
    mainWindow->m_centralWidget->cameraSelected(Scene::currentScene()->getCamera(), true);
}

void MainMenuBar::fileOpen() {
    QString filepath = QFileDialog::getOpenFileName(this, "Load Model(s)", "", "All Files (*)");
    Model* loadedModel = Loader::loadFromFile(filepath);
    if (loadedModel == 0) return;
    Scene::currentScene()->addModel(loadedModel);
    Scene::currentScene()->getCamera()->setDirection(loadedModel->getCenterOfMass() - Scene::currentScene()->getCamera()->getPosition());
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::fileExit() {
    QApplication::quit();
}

void MainMenuBar::createLight() {
    if (Scene::currentScene()->getLights().size() >= 8) {
        QMessageBox::critical(0, "Error", "The number of lights has reached the upper limit.");
        return;
    }
    Scene::currentScene()->addLight(new Light(QVector3D(0, 3, 0)));
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::createBasicCone() {
    Scene::currentScene()->addModel(Helper::createConeModel());
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::createBasicCube() {
    Scene::currentScene()->addModel(Helper::createCubeModel());
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::createBasicCylinder() {
    Scene::currentScene()->addModel(Helper::createCylinderModel());
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::createBasicPlane() {
    Scene::currentScene()->addModel(Helper::createPlaneModel());
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::createBasicSphere() {
    Scene::currentScene()->addModel(Helper::createSphereModel());
    mainWindow->m_centralWidget->sceneTreeView->reset();
}

void MainMenuBar::optionLighting(bool enabled) {
    OpenGLConfig::setEnableLighting(enabled);
}

void MainMenuBar::optionGridline(bool enabled) {
    OpenGLConfig::setEnableGridline(enabled);
}

void MainMenuBar::optionWireFrame(bool enabled) {
    OpenGLConfig::setEnableWireFrame(enabled);
}

void MainMenuBar::setFPS30() {
    actionSetFPS30->setChecked(true);
    actionSetFPS60->setChecked(false);
    mainWindow->m_centralWidget->openGLWidget->setFPSLimit(30);
}

void MainMenuBar::setFPS60() {
    actionSetFPS30->setChecked(false);
    actionSetFPS60->setChecked(true);
    mainWindow->m_centralWidget->openGLWidget->setFPSLimit(60);
}

void MainMenuBar::helpAbout() {
    QString info = "Current version: " + QString(APP_VERSION) + "\n\n";
    info += "masterEngine is a cross-platform 3D engine for learning purpose, based on Qt, OpenGL and Assimp.\n\n";
    info += "Author: Alfred Liu\n";
    info += "Email:  afterthat97@foxmail.com";
    QMessageBox::about(this, "About", info);
}

void MainMenuBar::helpCheckForUpdates() {
    QString url = "https://api.github.com/repos/afterthat97/masterEngine/releases/latest";
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyOfUpdates(QNetworkReply*)));
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void MainMenuBar::replyOfUpdates(QNetworkReply* reply) {
    QString strReply = (QString) reply->readAll();
    QJsonObject jsonObject = QJsonDocument::fromJson(strReply.toUtf8()).object();

    QString latestVersion = jsonObject["tag_name"].toString();
    if (latestVersion != APP_VERSION && latestVersion != "") {
        QString info = "A new version has been released, do you want to upgrade?\n\n";
        info += "Current version: " + QString(APP_VERSION) + "\n";
        info += "Latest version: " + latestVersion;
        if (QMessageBox::question(this, "Update", info, QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl(jsonObject["html_url"].toString()));
        }
    }
}
