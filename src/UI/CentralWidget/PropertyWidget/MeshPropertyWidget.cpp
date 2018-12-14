#include <UI/CentralWidget/PropertyWidget/MeshPropertyWidget.h>

MeshPropertyWidget::MeshPropertyWidget(Mesh* _mesh, QWidget *parent): QWidget(parent) {
    mesh = _mesh;

    meshVisibleCheckbox = new QCheckBox("Visible", this);
    meshVisibleCheckbox->setChecked(mesh->isVisible());

    meshPositionWidget = new Vector3DWidget("Position", false, this);
    meshPositionWidget->setLabelText("X", "Y", "Z");
    meshPositionWidget->setRange(-1000000.0, 1000000.0);
    meshPositionWidget->setSingleStep(1.0);
    meshPositionWidget->setValue(mesh->getPosition());

    meshRotationWidget = new Vector3DWidget("Rotation", true, this);
    meshRotationWidget->setLabelText("X", "Y", "Z");
    meshRotationWidget->setRange(0.0, 360.0);
    meshRotationWidget->setSingleStep(1.0);
    meshRotationWidget->setValue(mesh->getRotation());

    meshScalingWidget = new Vector3DWidget("Scaling", false, this);
    meshScalingWidget->setLabelText("X", "Y", "Z");
    meshScalingWidget->setRange(0.0, 1000000.0);
    meshScalingWidget->setSingleStep(1.0);
    meshScalingWidget->setValue(mesh->getScaling());

    setupLayout();
    setupSignals();
}

MeshPropertyWidget::~MeshPropertyWidget() {
    delete meshVisibleCheckbox;
    delete meshPositionWidget;
    delete meshRotationWidget;
    delete meshScalingWidget;
    delete mainLayout;
}

void MeshPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(meshVisibleCheckbox);
    mainLayout->addWidget(meshPositionWidget);
    mainLayout->addWidget(meshRotationWidget);
    mainLayout->addWidget(meshScalingWidget);
    setLayout(mainLayout);
}

void MeshPropertyWidget::setupSignals() {
    connect(meshVisibleCheckbox, SIGNAL(stateChanged(int)), this, SLOT(setMeshVisible(int)));
    connect(meshPositionWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setMeshPosition(QVector3D)));
    connect(meshRotationWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setMeshRotation(QVector3D)));
    connect(meshScalingWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setMeshScaling(QVector3D)));
}

void MeshPropertyWidget::setMeshVisible(int state) {
    mesh->setVisible(state == Qt::Checked);
}

void MeshPropertyWidget::setMeshPosition(QVector3D value) {
    mesh->setPosition(value);
}

void MeshPropertyWidget::setMeshRotation(QVector3D value) {
    mesh->setRotation(value);
}

void MeshPropertyWidget::setMeshScaling(QVector3D value) {
    mesh->setScaling(value);
}
