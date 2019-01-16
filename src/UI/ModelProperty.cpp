#include <UI/ModelProperty.h>

ModelProperty::ModelProperty(Model * model, QWidget * parent): QWidget(parent) {
    m_host = model;

    m_visibleCheckBox = new QCheckBox("Visible", this);
    m_positionEdit = new Vector3DEdit("Position", Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);
    m_rotationEditSlider = new Vector3DEditSlider("Rotation", Qt::Horizontal, "X", "Y", "Z", -180.0f, 180.0f, 2, this);
    m_scalingEdit = new Vector3DEdit("Scaling", Qt::Horizontal, "X", "Y", "Z", 0.0f, inf, 2, this);

    m_visibleCheckBox->setChecked(m_host->visible());
    m_positionEdit->setValue(m_host->localPosition());
    m_rotationEditSlider->setValue(m_host->localRotation());
    m_scalingEdit->setValue(m_host->localScaling());

    configLayout();
    configSignals();
}

void ModelProperty::configLayout() {
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_visibleCheckBox);
    mainLayout->addWidget(m_positionEdit);
    mainLayout->addWidget(m_rotationEditSlider);
    mainLayout->addWidget(m_scalingEdit);
    setLayout(mainLayout);
}

void ModelProperty::configSignals() {
    connect(m_visibleCheckBox, SIGNAL(toggled(bool)), m_host, SLOT(setVisible(bool)));
    connect(m_positionEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setPosition(QVector3D)));
    connect(m_rotationEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setRotation(QVector3D)));
    connect(m_scalingEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setScaling(QVector3D)));

    connect(m_host, SIGNAL(visibleChanged(bool)), m_visibleCheckBox, SLOT(setChecked(bool)));
    connect(m_host, SIGNAL(positionChanged(QVector3D)), m_positionEdit, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(rotationChanged(QVector3D)), m_rotationEditSlider, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(scalingChanged(QVector3D)), m_scalingEdit, SLOT(setValue(QVector3D)));
}
