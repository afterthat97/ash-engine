#include <UI/CentralWidget/PropertyWidget/ScenePropertyWidget.h>

// ScenePropertyWidget constructor

ScenePropertyWidget::ScenePropertyWidget(Scene *_scene, QWidget * parent): QWidget(parent) {
    scene = _scene;
    
    nameEditWidget = new StringEditWidget("Scene name: ", this);
    nameEditWidget->setValue(scene->objectName());

    cameraPropertyWidget = new CameraPropertyWidget(scene->getCamera(), this);

    setupLayout();
    setupSignals();
}

ScenePropertyWidget::~ScenePropertyWidget() {
    delete nameEditWidget;
    delete cameraPropertyWidget;
    delete mainLayout;
}

// Private functions

void ScenePropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(nameEditWidget);
    mainLayout->addWidget(cameraPropertyWidget);
    setLayout(mainLayout);
}

void ScenePropertyWidget::setupSignals() {
    connect(nameEditWidget, SIGNAL(valueChanged(QString)), this, SLOT(setSceneName(QString)));
}

// Private slots

void ScenePropertyWidget::setSceneName(QString name) {
    scene->setObjectName(name);
}

// CameraPropertyWidget constructor

CameraPropertyWidget::CameraPropertyWidget(Camera * _camera, QWidget * parent): QGroupBox(parent) {
    camera = _camera;
    setTitle("Camera");
    
    movingSpeedEditWidget = new FloatEditWidget("Moving speed: ", false, this);
    movingSpeedEditWidget->setRange(0.0, 1000000.0);
    movingSpeedEditWidget->setSingleStep(0.1);
    movingSpeedEditWidget->setValue(camera->getMovingSpeed());

    positionEditWidget = new Vector3DEditWidget("Position", false, this);
    positionEditWidget->setRange(-1000000.0, 1000000.0);
    positionEditWidget->setSingleStep(1.0);
    positionEditWidget->setValue(camera->getPosition());

    directionEditWidget = new Vector3DEditWidget("Direction", false, this);
    directionEditWidget->setRange(-1.0, 1.0);
    directionEditWidget->setSingleStep(0.01);
    directionEditWidget->setValue(camera->getDirection());

    setupLayout();
    setupSignals();
}

CameraPropertyWidget::~CameraPropertyWidget() {
    delete movingSpeedEditWidget;
    delete positionEditWidget;
    delete directionEditWidget;
    delete mainLayout;
}

// Private functions

void CameraPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(movingSpeedEditWidget);
    mainLayout->addWidget(positionEditWidget);
    mainLayout->addWidget(directionEditWidget);
    setLayout(mainLayout);
}

void CameraPropertyWidget::setupSignals() {
    // UI -> Camera
    connect(movingSpeedEditWidget, SIGNAL(valueChanged(float)), camera, SLOT(setMovingSpeed(float)));
    connect(positionEditWidget, SIGNAL(valueChanged(QVector3D)), camera, SLOT(setPosition(QVector3D)));
    connect(directionEditWidget, SIGNAL(valueChanged(QVector3D)), camera, SLOT(setDirection(QVector3D)));
    // Camera -> UI
    connect(camera, SIGNAL(positionChanged(QVector3D)), positionEditWidget, SLOT(setValue(QVector3D)));
    connect(camera, SIGNAL(directionChanged(QVector3D)), directionEditWidget, SLOT(setValue(QVector3D)));
}