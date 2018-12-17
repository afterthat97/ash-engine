#include <UI/CentralWidget/PropertyWidget/CameraPropertyWidget.h>

CameraPropertyWidget::CameraPropertyWidget(Camera * _camera, QWidget * parent): QWidget(parent) {
    camera = _camera;

    movingSpeedEditWidget = new FloatEditWidget("Moving Speed: ", false, this);
    movingSpeedEditWidget->setRange(0.0, 1000000.0);
    movingSpeedEditWidget->setSingleStep(0.1);
    movingSpeedEditWidget->setValue(camera->getMovingSpeed());

    positionEditWidget = new Vector3DEditWidget("Camera Position", false, this);
    positionEditWidget->setRange(-1000000.0, 1000000.0);
    positionEditWidget->setSingleStep(1.0);
    positionEditWidget->setValue(camera->getPosition());

    directionEditWidget = new Vector3DEditWidget("Camera Direction", false, this);
    directionEditWidget->setRange(-1.0, 1.0);
    directionEditWidget->setSingleStep(0.01);
    directionEditWidget->setValue(camera->getDirection());

    resetButton = new QPushButton("Reset Camera", this);

    setupLayout();
    setupSignals();
}

CameraPropertyWidget::~CameraPropertyWidget() {
    delete movingSpeedEditWidget;
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
    mainLayout->addWidget(positionEditWidget);
    mainLayout->addWidget(directionEditWidget);
    mainLayout->addWidget(resetButton);
    setLayout(mainLayout);
}

void CameraPropertyWidget::setupSignals() {
    // UI -> Camera
    connect(movingSpeedEditWidget, SIGNAL(valueChanged(float)), camera, SLOT(setMovingSpeed(float)));
    connect(positionEditWidget, SIGNAL(valueChanged(QVector3D)), camera, SLOT(setPosition(QVector3D)));
    connect(directionEditWidget, SIGNAL(valueChanged(QVector3D)), camera, SLOT(setDirection(QVector3D)));
    connect(resetButton, SIGNAL(clicked(bool)), camera, SLOT(reset()));

    // Camera -> UI
    connect(camera, SIGNAL(positionChanged(QVector3D)), positionEditWidget, SLOT(setValue(QVector3D)));
    connect(camera, SIGNAL(directionChanged(QVector3D)), directionEditWidget, SLOT(setValue(QVector3D)));
}
