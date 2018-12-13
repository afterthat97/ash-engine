#include <UI/MainMenuBar.h>
#include <Generic/Scene.h>
#include <Generic/Helper.h>
#include <IO/Loader.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLConfig.h>

MainMenuBar::MainMenuBar(QWidget *_parent): QMenuBar(_parent) {
    createActions();
    createMenus();
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
    actionFileNew = new QAction(QStringLiteral("新场景"), this);
    actionFileOpen = new QAction(QStringLiteral("打开"), this);
    actionFileExit = new QAction(QStringLiteral("退出"), this);

    actionCreateLight = new QAction(QStringLiteral("光源"), this);
    actionCreateBasicCube = new QAction(QStringLiteral("立方体"), this);
    actionCreateBasicSphere = new QAction(QStringLiteral("球体"), this);
    actionCreateBasicPlane = new QAction(QStringLiteral("平面"), this);

    actionOptionEnableLighting = new QAction(QStringLiteral("启用光照"), this);
    actionOptionEnableGridline = new QAction(QStringLiteral("启用网格线"), this);
    actionOptionEnableWireFrame = new QAction(QStringLiteral("启用线框模式"), this);
    actionOptionEnableLighting->setCheckable(true);
    actionOptionEnableGridline->setCheckable(true);
    actionOptionEnableWireFrame->setCheckable(true);
    actionOptionEnableLighting->setChecked(OpenGLConfig::isLightingEnabled());
    actionOptionEnableGridline->setChecked(OpenGLConfig::isGridlineEnabled());
    actionOptionEnableWireFrame->setChecked(OpenGLConfig::isWireFrameEnabled());

    actionHelpAbout = new QAction(QStringLiteral("关于"), this);
    actionHelpCheckUpdate = new QAction(QStringLiteral("检查更新"), this);

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
    menuFile = this->addMenu(QStringLiteral("文件"));
    menuFile->addAction(actionFileNew);
    menuFile->addAction(actionFileOpen);
    menuFile->addAction(actionFileExit);

    menuCreate = this->addMenu(QStringLiteral("创建"));
    menuCreate->addAction(actionCreateLight);

    menuCreateBasicShapes = menuCreate->addMenu(QStringLiteral("基本体"));
    menuCreateBasicShapes->addAction(actionCreateBasicCube);
    menuCreateBasicShapes->addAction(actionCreateBasicSphere);
    menuCreateBasicShapes->addAction(actionCreateBasicPlane);

    menuOptions = this->addMenu(QStringLiteral("选项"));
    menuOptions->addAction(actionOptionEnableLighting);
    menuOptions->addAction(actionOptionEnableGridline);
    menuOptions->addAction(actionOptionEnableWireFrame);

    menuHelp = this->addMenu(QStringLiteral("帮助"));
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
    QString filepath = QFileDialog::getOpenFileName(this, QStringLiteral("加载模型文件"), "", "All Files (*)");
    Scene::currentScene()->addModel(Loader::loadFromFile(filepath));
    sceneChanged();
}

void MainMenuBar::fileExit() {
    exit(0);
}

void MainMenuBar::createLight() {
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
    QString info = QStringLiteral("当前版本: ") + APP_VERSION + "\n\n";
    info += QStringLiteral("masterEngine 是一个跨平台的 3D 引擎，基于 Qt，OpenGL 和 Assimp，仅用于学习用途。\n\n");
    info += QStringLiteral("作者: Alfred Liu\n");
    info += QStringLiteral("邮箱: afterthat97@foxmail.com");
    QMessageBox::about(this, QStringLiteral("关于"), info);
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
    if (latestVersion != APP_VERSION) {
        QString info = QStringLiteral("检测到新版本，是否更新？\n\n");
        info += QStringLiteral("当前版本: ") + APP_VERSION + "\n";
        info += QStringLiteral("最新版本: ") + latestVersion;
        if (QMessageBox::question(this, QStringLiteral("更新"), info, QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl(jsonObject["html_url"].toString()));
        }
    }
}
