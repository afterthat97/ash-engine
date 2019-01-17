#include <UI/CameraProperty.h>

CameraProperty::CameraProperty(Camera * camera, QWidget * parent): QWidget(parent) {
    m_host = camera;

    m_movingSpeedLabel = new QLabel("Moving Speed:",  this);
    m_fieldOfViewLabel = new QLabel("Field of View:", this);
    m_nearPlaneLabel   = new QLabel("Near Plane:",    this);
    m_farPlaneLabel    = new QLabel("Far Plane:",     this);

    m_movingSpeedEdit  = new FloatEdit(0.0f, inf, 2, this);
    m_fieldOfViewEdit  = new FloatEdit(0.0f, inf, 1, this);
    m_nearPlaneEdit    = new FloatEdit(0.0f, inf, 2, this);
    m_farPlaneEdit     = new FloatEdit(0.0f, inf, 1, this);

    m_movingSpeedSlider = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f,   this);
    m_fieldOfViewSlider = new FloatSlider(Qt::Horizontal, 0.0f, 180.0f, this);

    m_positionEdit  = new Vector3DEdit("Position",  Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);
    m_directionEdit = new Vector3DEdit("Direction", Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);

    m_resetButton = new QPushButton("Reset Camera", this);

    m_movingSpeedEdit->setValue(m_host->movingSpeed());
    m_fieldOfViewEdit->setValue(m_host->fieldOfView());
    m_nearPlaneEdit->setValue(m_host->nearPlane());
    m_farPlaneEdit->setValue(m_host->farPlane());
    m_movingSpeedSlider->setValue(m_host->movingSpeed());
    m_fieldOfViewSlider->setValue(m_host->fieldOfView());
    m_positionEdit->setValue(m_host->position());
    m_directionEdit->setValue(m_host->direction());

    configLayout();
    configSignals();
}

// Private functions

void CameraProperty::configLayout() {
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_movingSpeedLabel, 0, 0);
    mainLayout->addWidget(m_movingSpeedEdit, 0, 1);
    mainLayout->addWidget(m_movingSpeedSlider, 0, 2);
    mainLayout->addWidget(m_fieldOfViewLabel, 1, 0);
    mainLayout->addWidget(m_fieldOfViewEdit, 1, 1);
    mainLayout->addWidget(m_fieldOfViewSlider, 1, 2);
    mainLayout->addWidget(m_nearPlaneLabel, 2, 0);
    mainLayout->addWidget(m_nearPlaneEdit, 2, 1);
    mainLayout->addWidget(m_farPlaneLabel, 3, 0);
    mainLayout->addWidget(m_farPlaneEdit, 3, 1);
    mainLayout->addWidget(m_positionEdit, 4, 0, 1, 3);
    mainLayout->addWidget(m_directionEdit, 5, 0, 1, 3);
    mainLayout->addWidget(m_resetButton, 6, 1);
    setLayout(mainLayout);
}

void CameraProperty::configSignals() {
    connect(m_movingSpeedEdit, SIGNAL(valueEdited(float)),     m_host, SLOT(setMovingSpeed(float)));
    connect(m_fieldOfViewEdit, SIGNAL(valueEdited(float)),     m_host, SLOT(setFieldOfView(float)));
    connect(m_nearPlaneEdit,   SIGNAL(valueEdited(float)),     m_host, SLOT(setNearPlane(float)));
    connect(m_farPlaneEdit,    SIGNAL(valueEdited(float)),     m_host, SLOT(setFarPlane(float)));
    connect(m_positionEdit,    SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setPosition(QVector3D)));
    connect(m_directionEdit,   SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setDirection(QVector3D)));
    connect(m_resetButton,     SIGNAL(clicked(bool)),          m_host, SLOT(reset()));

    connect(m_movingSpeedSlider, SIGNAL(valueSlided(float)), m_host, SLOT(setMovingSpeed(float)));
    connect(m_fieldOfViewSlider, SIGNAL(valueSlided(float)), m_host, SLOT(setFieldOfView(float)));

    connect(m_host, SIGNAL(movingSpeedChanged(float)),   m_movingSpeedEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(fieldOfViewChanged(float)),   m_fieldOfViewEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(nearPlaneChanged(float)),     m_nearPlaneEdit,   SLOT(setValue(float)));
    connect(m_host, SIGNAL(farPlaneChanged(float)),      m_farPlaneEdit,    SLOT(setValue(float)));
    connect(m_host, SIGNAL(positionChanged(QVector3D)),  m_positionEdit,    SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(directionChanged(QVector3D)), m_directionEdit,   SLOT(setValue(QVector3D)));

    connect(m_host, SIGNAL(movingSpeedChanged(float)), m_movingSpeedSlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(fieldOfViewChanged(float)), m_fieldOfViewSlider, SLOT(setValue(float)));
}
