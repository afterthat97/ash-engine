#include <UI/CentralWidget/CentralWidget.h>
#include <UI/MainWindow.h>

CentralWidget::CentralWidget(MainWindow * parent): QWidget(parent), mainWindow(parent) {
    sceneTreeView = new SceneTreeView(mainWindow, this);
    openGLWidget = new OpenGLWidget(mainWindow, this);
    propertyWidget = new EmptyPropertyWidget(this);

    splitter = new QSplitter(this);
    splitter->addWidget(sceneTreeView);
    splitter->addWidget(openGLWidget);
    splitter->addWidget(propertyWidget);
    splitter->setSizes(QList<int>{200, 500, 260});

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget() {
    delete sceneTreeView;
    delete openGLWidget;
    delete propertyWidget;
    delete splitter;
    delete mainLayout;
}

void CentralWidget::resetPropertyWidget() {
    propertyWidget = new EmptyPropertyWidget(this);
    delete splitter->replaceWidget(2, propertyWidget);
}

void CentralWidget::setPropertyWidget(Model * model) {
    propertyWidget = new EmptyPropertyWidget(this);
    delete splitter->replaceWidget(2, propertyWidget);
}

void CentralWidget::setPropertyWidget(Light * light) {
    propertyWidget = new LightPropertyWidget(light, this);
    delete splitter->replaceWidget(2, propertyWidget);
}

void CentralWidget::setPropertyWidget(Mesh * mesh) {
    propertyWidget = new MeshPropertyWidget(mesh, this);
    delete splitter->replaceWidget(2, propertyWidget);
}

void CentralWidget::setPropertyWidget(Material * material) {
    propertyWidget = new MaterialPropertyWidget(material, this);
    delete splitter->replaceWidget(2, propertyWidget);
}

void CentralWidget::setPropertyWidget(Texture * texture) {
    propertyWidget = new TexturePropertyWidget(texture, this);
    delete splitter->replaceWidget(2, propertyWidget);
}
