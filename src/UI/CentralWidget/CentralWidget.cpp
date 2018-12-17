#include <UI/CentralWidget/CentralWidget.h>
#include <UI/MainWindow.h>

CentralWidget::CentralWidget(MainWindow * parent): QWidget(parent), mainWindow(parent) {
    sceneTreeView = new SceneTreeView(this);
    openGLWidget = new OpenGLWidget(this);
    propertyWidget = new QWidget(this);

    setupLayout();
    setupSignals();
}

CentralWidget::~CentralWidget() {
    delete sceneTreeView;
    delete openGLWidget;
    delete propertyWidget;
    delete splitter;
    delete mainLayout;
}

// Private functions

void CentralWidget::setupLayout() {
    splitter = new QSplitter(this);
    splitter->addWidget(sceneTreeView);
    splitter->addWidget(openGLWidget);
    splitter->addWidget(propertyWidget);
    splitter->setSizes(QList<int>{160, 500, 340});

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);
}

void CentralWidget::setupSignals() {
    connect(openGLWidget, SIGNAL(FPSChanged(uint32_t)), this, SLOT(FPSChanged(uint32_t)));
    connect(sceneTreeView, SIGNAL(cameraSelected(Camera*, bool)), this, SLOT(cameraSelected(Camera*,bool)));
    connect(sceneTreeView, SIGNAL(modelSelected(Model*, bool)), this, SLOT(modelSelected(Model*, bool)));
    connect(sceneTreeView, SIGNAL(lightSelected(Light*, bool)), this, SLOT(lightSelected(Light*, bool)));
    connect(sceneTreeView, SIGNAL(meshSelected(Mesh*, bool)), this, SLOT(meshSelected(Mesh*, bool)));
    connect(sceneTreeView, SIGNAL(materialSelected(Material*, bool)), this, SLOT(materialSelected(Material*, bool)));
    connect(sceneTreeView, SIGNAL(textureSelected(Texture*, bool)), this, SLOT(textureSelected(Texture*, bool)));
}

// Private slots

void CentralWidget::FPSChanged(uint32_t FPS) {
    mainWindow->statusBar()->showMessage("FPS: " + QString::number(FPS));
}

void CentralWidget::cameraSelected(Camera* camera, bool selected) {
    if (selected) {
        propertyWidget = new CameraPropertyWidget(camera, this);
        delete splitter->replaceWidget(2, propertyWidget);
    }
}

void CentralWidget::modelSelected(Model* model, bool selected) {
    if (selected) {
        propertyWidget = new ModelPropertyWidget(model, this);
        delete splitter->replaceWidget(2, propertyWidget);
    }
}

void CentralWidget::lightSelected(Light * light, bool selected) {
    if (selected) {
        propertyWidget = new LightPropertyWidget(light, this);
        delete splitter->replaceWidget(2, propertyWidget);
    }
}

void CentralWidget::meshSelected(Mesh * mesh, bool selected) {
    if (selected) {
        propertyWidget = new MeshPropertyWidget(mesh, this);
        delete splitter->replaceWidget(2, propertyWidget);
    }
}

void CentralWidget::materialSelected(Material * material, bool selected) {
    if (selected) {
        propertyWidget = new MaterialPropertyWidget(material, this);
        delete splitter->replaceWidget(2, propertyWidget);
    }
}

void CentralWidget::textureSelected(Texture * texture, bool selected) {
    if (selected) {
        propertyWidget = new TexturePropertyWidget(texture, this);
        delete splitter->replaceWidget(2, propertyWidget);
    }
}
