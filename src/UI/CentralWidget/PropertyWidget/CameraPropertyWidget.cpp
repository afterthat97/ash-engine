#include <UI/CentralWidget/PropertyWidget/CameraPropertyWidget.h>

CameraPropertyWidget::CameraPropertyWidget(Camera * _camera, QWidget * parent): QWidget(parent) {
    camera = _camera;

    movingSpeedEditWidget = new FloatEditWidget("Moving Speed: ", false, this);
    movingSpeedEditWidget->setLabelWidget(90);
    movingSpeedEditWidget->setRange(0.0, 1000000.0);
    movingSpeedEditWidget->setSingleStep(0.1);
    movingSpeedEditWidget->setValue(camera->getMovingSpeed());

    fieldOfViewEditWidget = new FloatEditWidget("Field of View: ", false, this);
    fieldOfViewEditWidget->setLabelWidget(90);
    fieldOfViewEditWidget->setRange(0.0, 1000000.0);
    fieldOfViewEditWidget->setSingleStep(0.1);
    fieldOfViewEditWidget->setValue(camera->getFieldOfView());

    nearPlaneEditWidget = new FloatEditWidget("Near Plane: ", false, this);
    nearPlaneEditWidget->setLabelWidget(90);
    nearPlaneEditWidget->setRange(0.0, 1000000.0);
    nearPlaneEditWidget->setSingleStep(0.01);
    nearPlaneEditWidget->setValue(camera->getNearPlane());

    farPlaneEditWidget = new FloatEditWidget("Far Plane: ", false, this);
    farPlaneEditWidget->setLabelWidget(90);
    farPlaneEditWidget->setRange(0.0, 1000000.0);
    farPlaneEditWidget->setSingleStep(1);
    farPlaneEditWidget->setValue(camera->getFarPlane());

    positionEditWidget = new Vector3DEditWidget("Camera Position", false, this);
    positionEditWidget->setLabelText("X", "Y", "Z");
    positionEditWidget->setRange(-1000000.0, 1000000.0);
    positionEditWidget->setSingleStep(1.0);
    positionEditWidget->setValue(camera->getPosition());

    directionEditWidget = new Vector3DEditWidget("Camera Direction", false, this);
    directionEditWidget->setLabelText("X", "Y", "Z");
    directionEditWidget->setRange(-1.0, 1.0);
    directionEditWidget->setSingleStep(0.01);
    directionEditWidget->setValue(camera->getDirection());

    resetButton = new QPushButton("Reset Camera", this);
    resetButton->setFixedWidth(240);

    setupLayout();
    setupSignals();
}

CameraPropertyWidget::~CameraPropertyWidget() {
    delete movingSpeedEditWidget;
    delete fieldOfViewEditWidget;
    delete nearPlaneEditWidget;
    delete farPlaneEditWidget;
    delete positionEditWidget;
    delete directionEditWidget;
    delete resetButton;
    delete mainLayout;
}

// Private functions

void CameraPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(movingSpeedEditWidget);
    mainLayout->addWidget(fieldOfViewEditWidget);
    mainLayout->addWidget(nearPlaneEditWidget);
    mainLayout->addWidget(farPlaneEditWidget);
    mainLayout->addWidget(positionEditWidget);
    mainLayout->addWidget(directionEditWidget);
    mainLayout->addWidget(resetButton, 0, Qt::AlignCenter);
    setLayout(mainLayout);
}

void CameraPropertyWidget::setupSignals() {
    // UI -> Camera
    connect(movingSpeedEditWidget, SIGNAL(valueChanged(float)), camera, SLOT(setMovingSpeed(float)));
    connect(fieldOfViewEditWidget, SIGNAL(valueChanged(float)), camera, SLOT(setFieldOfView(float)));
    connect(nearPlaneEditWidget, SIGNAL(valueChanged(float)), camera, SLOT(setNearPlane(float)));
    connect(farPlaneEditWidget, SIGNAL(valueChanged(float)), camera, SLOT(setFarPlane(float)));
    connect(positionEditWidget, SIGNAL(valueChanged(QVector3D)), camera, SLOT(setPosition(QVector3D)));
    connect(directionEditWidget, SIGNAL(valueChanged(QVector3D)), camera, SLOT(setDirection(QVector3D)));
    connect(resetButton, SIGNAL(clicked(bool)), camera, SLOT(reset()));

    // Camera -> UI
    connect(camera, SIGNAL(positionChanged(QVector3D)), positionEditWidget, SLOT(setValue(QVector3D)));
    connect(camera, SIGNAL(directionChanged(QVector3D)), directionEditWidget, SLOT(setValue(QVector3D)));
}
