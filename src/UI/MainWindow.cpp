#include <MainWindow.h>
#include <CameraProperty.h>
#include <GridlineProperty.h>
#include <AmbientLightProperty.h>
#include <DirectionalLightProperty.h>
#include <PointLightProperty.h>
#include <SpotLightProperty.h>
#include <ModelProperty.h>
#include <MeshProperty.h>
#include <MaterialProperty.h>
#include <ModelLoader.h>
#include <ModelExporter.h>
#include <SceneLoader.h>
#include <SceneSaver.h>

MainWindow::MainWindow(Scene * scene, QWidget * parent): QMainWindow(parent) {
    m_host = scene;
    m_copyedObject = 0;
    m_fpsLabel = new QLabel(this);
    m_sceneTreeWidget = new SceneTreeWidget(m_host, this);
    m_openGLWindow = new OpenGLWindow(new OpenGLScene(m_host), new OpenGLRenderer);
    m_propertyWidget = new QScrollArea(this);
    m_propertyWidget->setWidgetResizable(true);

    statusBar()->addPermanentWidget(m_fpsLabel);

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(new QWidget);

    configMenu();
    configLayout();
    configSignals();

    resize(1200, 720);
    helpCheckForUpdates();
}

void MainWindow::resizeEvent(QResizeEvent *) {
    m_splitter->setSizes(QList<int>{160, width() - 160 - 300, 300});
}

bool MainWindow::askToSaveScene() {
    int answer = QMessageBox::question(this,
                                       "Unsaved scene",
                                       "Save current scene? Any unsaved changes will be lost.",
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (answer == QMessageBox::Cancel)
        return false;
    if (answer == QMessageBox::Yes)
        fileSaveScene();
    return true;
}

void MainWindow::configMenu() {
    QMenu *menuFile = menuBar()->addMenu("File");
    menuFile->addAction("New Scene", this, SLOT(fileNewScene()), QKeySequence(Qt::CTRL + Qt::Key_N));
    menuFile->addAction("Open Scene", this, SLOT(fileOpenScene()), QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addSeparator();
    menuFile->addAction("Import Model", this, SLOT(fileImportModel()));
    menuFile->addAction("Export Model", this, SLOT(fileExportModel()));
    menuFile->addSeparator();
    menuFile->addAction("Save Scene", this, SLOT(fileSaveScene()), QKeySequence(Qt::CTRL + Qt::Key_S));
    menuFile->addAction("Save Scene As", this, SLOT(fileSaveAsScene()));
    menuFile->addSeparator();
    menuFile->addAction("Quit", this, SLOT(fileQuit()), QKeySequence(Qt::CTRL + Qt::Key_Q));

    QMenu *menuEdit = menuBar()->addMenu("Edit");
    menuEdit->addAction("Copy", this, SLOT(editCopy()), QKeySequence(Qt::CTRL + Qt::Key_C));
    menuEdit->addAction("Paste", this, SLOT(editPaste()), QKeySequence(Qt::CTRL + Qt::Key_V));
    menuEdit->addSeparator();
    menuEdit->addAction("Remove", this, SLOT(editRemove()), QKeySequence(Qt::Key_Backspace));

    QMenu *menuCreate = menuBar()->addMenu("Create");
    menuCreate->addAction("Gridline", this, SLOT(createGridline()));

    QMenu *menuCreateLight = menuCreate->addMenu("Light");
    menuCreateLight->addAction("Ambient Light", this, SLOT(createAmbientLight()));
    menuCreateLight->addAction("Directional Light", this, SLOT(createDirectionalLight()));
    menuCreateLight->addAction("Point Light", this, SLOT(createPointLight()));
    menuCreateLight->addAction("Spot Light", this, SLOT(createSpotLight()));

    QMenu *menuCreateBasicShapes = menuCreate->addMenu("Basic Shape");
    menuCreateBasicShapes->addAction("Cone", this, SLOT(createBasicCone()));
    menuCreateBasicShapes->addAction("Cube", this, SLOT(createBasicCube()));
    menuCreateBasicShapes->addAction("Cylinder", this, SLOT(createBasicCylinder()));
    menuCreateBasicShapes->addAction("Plane", this, SLOT(createBasicPlane()));
    menuCreateBasicShapes->addAction("Sphere", this, SLOT(createBasicSphere()));

    QMenu *menuAxisType = menuBar()->addMenu("Axis Type");
    QAction *actionAxisTypeTranslate = menuAxisType->addAction("Translate", this, SLOT(setAxisTypeTranslate()), QKeySequence(Qt::CTRL + Qt::Key_F1));
    QAction *actionAxisTypeRotate = menuAxisType->addAction("Rotate", this, SLOT(setAxisTypeRotate()), QKeySequence(Qt::CTRL + Qt::Key_F2));
    QAction *actionAxisTypeScale = menuAxisType->addAction("Scale", this, SLOT(setAxisTypeScale()), QKeySequence(Qt::CTRL + Qt::Key_F3));
    actionAxisTypeTranslate->setCheckable(true);
    actionAxisTypeRotate->setCheckable(true);
    actionAxisTypeScale->setCheckable(true);

    QActionGroup *actionAxisTypeGroup = new QActionGroup(menuAxisType);
    actionAxisTypeGroup->addAction(actionAxisTypeTranslate);
    actionAxisTypeGroup->addAction(actionAxisTypeRotate);
    actionAxisTypeGroup->addAction(actionAxisTypeScale);
    actionAxisTypeTranslate->setChecked(true);

    QMenu *menuHelp = menuBar()->addMenu("Help");
    menuHelp->addAction("Check for Update", this, SLOT(helpCheckForUpdates()));
    menuHelp->addAction("View Source Code", this, SLOT(helpSourceCode()));
    menuHelp->addSeparator();
    menuHelp->addAction("Bug Report", this, SLOT(helpBugReport()));
    menuHelp->addAction("Feature Request", this, SLOT(helpFeatureRequest()));
    menuHelp->addSeparator();
    menuHelp->addAction("About", this, SLOT(helpAbout()));
}

void MainWindow::configLayout() {
    m_splitter = new QSplitter(this);
    m_splitter->addWidget(m_sceneTreeWidget);
    m_splitter->addWidget(QWidget::createWindowContainer(m_openGLWindow));
    m_splitter->addWidget(m_propertyWidget);
    m_splitter->setSizes(QList<int>{160, width() - 160 - 300, 300});

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_splitter);
    centralWidget()->setLayout(mainLayout);
}

void MainWindow::configSignals() {
    connect(m_openGLWindow, SIGNAL(fpsChanged(int)), this, SLOT(fpsChanged(int)));
    connect(m_sceneTreeWidget, SIGNAL(itemSelected(QVariant)), this, SLOT(itemSelected(QVariant)));
    connect(m_sceneTreeWidget, SIGNAL(itemDeselected(QVariant)), this, SLOT(itemDeselected(QVariant)));
}

void MainWindow::fpsChanged(int fps) {
    m_fpsLabel->setText("FPS: " + QString::number(fps));
}

void MainWindow::itemSelected(QVariant item) {
    delete m_propertyWidget->takeWidget();
    if (item.canConvert<Camera*>()) {
        m_propertyWidget->setWidget(new CameraProperty(item.value<Camera*>(), this));
    } else if (item.canConvert<Gridline*>()) {
        m_propertyWidget->setWidget(new GridlineProperty(item.value<Gridline*>(), this));
    } else if (item.canConvert<AmbientLight*>()) {
        m_propertyWidget->setWidget(new AmbientLightProperty(item.value<AmbientLight*>(), this));
    } else if (item.canConvert<DirectionalLight*>()) {
        m_propertyWidget->setWidget(new DirectionalLightProperty(item.value<DirectionalLight*>(), this));
    } else if (item.canConvert<PointLight*>()) {
        m_propertyWidget->setWidget(new PointLightProperty(item.value<PointLight*>(), this));
    } else if (item.canConvert<SpotLight*>()) {
        m_propertyWidget->setWidget(new SpotLightProperty(item.value<SpotLight*>(), this));
    } else if (item.canConvert<Model*>()) {
        m_propertyWidget->setWidget(new ModelProperty(item.value<Model*>(), this));
    } else if (item.canConvert<Mesh*>()) {
        m_propertyWidget->setWidget(new MeshProperty(item.value<Mesh*>(), this));
    } else if (item.canConvert<Material*>()) {
        m_propertyWidget->setWidget(new MaterialProperty(item.value<Material*>(), this));
    }
}

void MainWindow::itemDeselected(QVariant) {
    m_propertyWidget->setWidget(0);
}

void MainWindow::fileNewScene() {
    if (!askToSaveScene()) return;
    delete m_propertyWidget->takeWidget();
    m_propertyWidget->setWidget(0);
    delete m_host;
    m_host = new Scene;
    m_host->addGridline(new Gridline);
    m_host->addDirectionalLight(new DirectionalLight);
    m_sceneTreeWidget->setScene(m_host);
    m_openGLWindow->setScene(new OpenGLScene(m_host));
    m_sceneFilePath = "";
}

void MainWindow::fileOpenScene() {
    if (!askToSaveScene()) return;
    QString filePath = QFileDialog::getOpenFileName(this, "Open Project", "", "Ash Engine Project (*.aeproj)");
    if (filePath == 0) return;
    SceneLoader loader;
    Scene* scene = loader.loadFromFile(filePath);
    if (scene == 0) {
        QString log = loader.log();
        qWarning() << "Failed to load" << filePath;
        qWarning() << log;
        QMessageBox::critical(0, "Failed to load file", log);
    } else {
        m_propertyWidget->setWidget(0);
        delete m_host;
        m_host = scene;
        m_sceneTreeWidget->setScene(m_host);
        m_openGLWindow->setScene(new OpenGLScene(m_host));
        m_sceneFilePath = filePath;
    }
}

void MainWindow::fileImportModel() {
    QString filePath = QFileDialog::getOpenFileName(this, "Load Model", "", "All Files (*)");
    if (filePath == 0) return;
    ModelLoader loader;
    Model* model = loader.loadModelFromFile(filePath);
    if (model == 0) {
        QString log = loader.log();
        qWarning() << "Failed to load" << filePath;
        qWarning() << log;
        QMessageBox::critical(0, "Failed to load file", log);
    } else
        m_host->addModel(model);
}

void MainWindow::fileExportModel() {
    if (AbstractEntity::getSelected() == 0 || qobject_cast<AbstractLight*>(AbstractEntity::getSelected()->parent())) {
        QMessageBox::warning(0, "Failed to export model", "Select a Model/Mesh to export.");
        return;
    }

    QString filter = "X Files (*.x);;";
    filter += "Step Files (*.stp);;";
    filter += "Wavefront OBJ format (*.obj);;";
    filter += "Stereolithography (*.stl);;";
    filter += "Stanford Polygon Library (*.ply);;";
    filter += "Autodesk 3DS (legacy) (*.3ds);;";
    filter += "Extensible 3D (*.x3d);;";
    filter += "3MF File Format (*.3mf);;";
    QString filePath = QFileDialog::getSaveFileName(this, "Export Model", "", filter);

    ModelExporter exporter;
    if (Model* model = qobject_cast<Model*>(AbstractEntity::getSelected()))
        exporter.saveToFile(model, filePath);
    else if (Mesh* mesh = qobject_cast<Mesh*>(AbstractEntity::getSelected()))
        exporter.saveToFile(mesh, filePath);

    if (exporter.hasLog()) {
        QString log = exporter.log();
        qWarning() << "Failed to export model to" << filePath;
        qWarning() << log;
        QMessageBox::critical(0, "Failed to export model", log);
    }
}

void MainWindow::fileSaveScene() {
    if (m_sceneFilePath.length()) {
        SceneSaver saver(m_host);
        saver.saveToFile(m_sceneFilePath);
    } else
        fileSaveAsScene();
}

void MainWindow::fileSaveAsScene() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save Project", "", "Ash Engine Project (*.aeproj)");
    SceneSaver saver(m_host);
    saver.saveToFile(filePath);
    m_sceneFilePath = filePath;
}

void MainWindow::fileQuit() {
    if (!askToSaveScene()) return;
    QApplication::quit();
}

void MainWindow::editCopy() {
    m_copyedObject = AbstractEntity::getSelected();
}

void MainWindow::editPaste() {
    if (m_copyedObject == 0) return;
    if (PointLight* light = qobject_cast<PointLight*>(m_copyedObject->parent())) {
        PointLight* newLight = new PointLight(*light);
        newLight->setParent(light->parent());
    } else if (SpotLight* light = qobject_cast<SpotLight*>(m_copyedObject->parent())) {
        SpotLight* newLight = new SpotLight(*light);
        newLight->setParent(light->parent());
    } else if (Model* model = qobject_cast<Model*>(m_copyedObject)) {
        Model* newModel = new Model(*model);
        newModel->setParent(model->parent());
    } else if (Mesh* mesh = qobject_cast<Mesh*>(m_copyedObject)) {
        Mesh* newMesh = new Mesh(*mesh);
        newMesh->setParent(mesh->parent());
    }
}

void MainWindow::editRemove() {
    if (!m_sceneTreeWidget->currentItem()) return;
    QVariant item = m_sceneTreeWidget->currentItem()->data(0, Qt::UserRole);
    if (item.value<QObject*>() == m_host->camera()) {
        QMessageBox::warning(this, "Warning", "Camera can not be deleted.");
        return;
    }
    itemDeselected(item);
    if (m_copyedObject == item.value<QObject*>())
        m_copyedObject = 0;
    delete item.value<QObject*>();
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

void MainWindow::setAxisTypeTranslate() {
    m_host->transformGizmo()->setTransformMode(TransformGizmo::Translate);
}

void MainWindow::setAxisTypeRotate() {
    m_host->transformGizmo()->setTransformMode(TransformGizmo::Rotate);
}

void MainWindow::setAxisTypeScale() {
    m_host->transformGizmo()->setTransformMode(TransformGizmo::Scale);
}

void MainWindow::helpAbout() {
    QString info = "Current version: " + QString(APP_VERSION) + "\n\n";
    info += "masterEngine is a cross-platform 3D engine for learning purpose, based on Qt, OpenGL and Assimp.\n\n";
    info += "Author: Alfred Liu\n";
    info += "Email:  afterthat97@foxmail.com";
    QMessageBox::about(this, "About", info);
}

void MainWindow::helpSourceCode() {
    QDesktopServices::openUrl(QUrl("https://github.com/afterthat97/masterEngine"));
}

void MainWindow::helpBugReport() {
    QDesktopServices::openUrl(QUrl("https://github.com/afterthat97/masterEngine/issues/new?template=bug_report.md"));
}

void MainWindow::helpFeatureRequest() {
    QDesktopServices::openUrl(QUrl("https://github.com/afterthat97/masterEngine/issues/new?template=feature_request.md"));
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
        if (QMessageBox::question(this, "New Update Available", info, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl(jsonObject["html_url"].toString()));
        }
    }
}
