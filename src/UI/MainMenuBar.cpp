#include <UI/MainMenuBar.h>
#include <Generic/Scene.h>
#include <Generic/Helper.h>
#include <IO/Loader.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLConfig.h>

MainMenuBar::MainMenuBar(QWidget *_parent): QMenuBar(_parent) {
    createActions();
    createMenus();
    helpCheckForUpdates();
}

MainMenuBar::~MainMenuBar() {
    delete menuFile;
    delete menuCreate;
    delete menuCreateBasicShapes;
    delete menuHelp;

    delete actionFileNew;
    delete actionFileOpen;
    delete actionFileExit;

    delete actionCreateLight;
    delete actionCreateBasicCube;
    delete actionCreateBasicSphere;
    delete actionCreateBasicPlane;

    delete actionOptionEnableLighting;
    delete actionOptionEnableGridline;
    delete actionOptionEnableWireFrame;

    delete actionHelpAbout;
    delete actionHelpCheckUpdate;
}

void MainMenuBar::createActions() {
    actionFileNew = new QAction("New", this);
    actionFileOpen = new QAction("Open", this);
    actionFileExit = new QAction("Exit", this);

    actionCreateLight = new QAction("Light", this);
    actionCreateBasicCube = new QAction("Cube", this);
    actionCreateBasicSphere = new QAction("Sphere", this);
    actionCreateBasicPlane = new QAction("Plane", this);

    actionOptionEnableLighting = new QAction("Enable Lighting", this);
    actionOptionEnableGridline = new QAction("Enable Gridline", this);
    actionOptionEnableWireFrame = new QAction("WireFrame Mode", this);
    actionOptionEnableLighting->setCheckable(true);
    actionOptionEnableGridline->setCheckable(true);
    actionOptionEnableWireFrame->setCheckable(true);
    actionOptionEnableLighting->setChecked(OpenGLConfig::isLightingEnabled());
    actionOptionEnableGridline->setChecked(OpenGLConfig::isGridlineEnabled());
    actionOptionEnableWireFrame->setChecked(OpenGLConfig::isWireFrameEnabled());

    actionHelpAbout = new QAction("About", this);
    actionHelpCheckUpdate = new QAction("Check for Update", this);

    connect(actionFileNew, SIGNAL(triggered(bool)), this, SLOT(fileNew()));
    connect(actionFileOpen, SIGNAL(triggered(bool)), this, SLOT(fileOpen()));
    connect(actionFileExit, SIGNAL(triggered(bool)), this, SLOT(fileExit()));

    connect(actionCreateLight, SIGNAL(triggered(bool)), this, SLOT(createLight()));
    connect(actionCreateBasicCube, SIGNAL(triggered(bool)), this, SLOT(createBasicCube()));
    connect(actionCreateBasicSphere, SIGNAL(triggered(bool)), this, SLOT(createBasicSphere()));
    connect(actionCreateBasicPlane, SIGNAL(triggered(bool)), this, SLOT(createBasicPlane()));

    connect(actionOptionEnableLighting, SIGNAL(triggered(bool)), this, SLOT(optionLighting(bool)));
    connect(actionOptionEnableGridline, SIGNAL(triggered(bool)), this, SLOT(optionGridline(bool)));
    connect(actionOptionEnableWireFrame, SIGNAL(triggered(bool)), this, SLOT(optionWireFrame(bool)));

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
    menuCreateBasicShapes->addAction(actionCreateBasicCube);
    menuCreateBasicShapes->addAction(actionCreateBasicSphere);
    menuCreateBasicShapes->addAction(actionCreateBasicPlane);

    menuOptions = this->addMenu("Options");
    menuOptions->addAction(actionOptionEnableLighting);
    menuOptions->addAction(actionOptionEnableGridline);
    menuOptions->addAction(actionOptionEnableWireFrame);

    menuHelp = this->addMenu("Help");
    menuHelp->addAction(actionHelpAbout);
    menuHelp->addAction(actionHelpCheckUpdate);
}

void MainMenuBar::fileNew() {
    sceneChanged();
    Scene::currentScene()->clean();
    OpenGLManager::cleanOpenGLMesh();
    OpenGLManager::cleanOpenGLTexture();
    sceneChanged();
}

void MainMenuBar::fileOpen() {
    QString filepath = QFileDialog::getOpenFileName(this, "Load Model(s)", "", "All Files (*)");
    Scene::currentScene()->addModel(Loader::loadFromFile(filepath));
    sceneChanged();
}

void MainMenuBar::fileExit() {
    exit(0);
}

void MainMenuBar::createLight() {
    if (Scene::currentScene()->getLights().size() >= 8) {
        QMessageBox::critical(0, "Error", "The number of lights has reached the upper limit.");
        return;
    }
    Scene::currentScene()->addLight(new Light(QVector3D(0, 100, 0)));
    sceneChanged();
}

void MainMenuBar::createBasicCube() {
    Scene::currentScene()->addModel(Helper::createCubeModel());
    sceneChanged();
}

void MainMenuBar::createBasicSphere() {
    Scene::currentScene()->addModel(Helper::createSphereModel());
    sceneChanged();
}

void MainMenuBar::createBasicPlane() {
    Scene::currentScene()->addModel(Helper::createPlaneModel());
    sceneChanged();
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
