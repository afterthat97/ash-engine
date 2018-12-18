#include <UI/CentralWidget/PropertyWidget/MeshPropertyWidget.h>

MeshPropertyWidget::MeshPropertyWidget(Mesh* _mesh, QWidget *parent): QWidget(parent) {
    mesh = _mesh;

    meshVisibleCheckbox = new QCheckBox("Mesh Visible", this);
    meshVisibleCheckbox->setChecked(mesh->isVisible());

    meshReverseNormalCheckBox = new QCheckBox("Reverse Normal", this);
    meshReverseNormalCheckBox->setChecked(mesh->isNormalReversed());

    meshPositionWidget = new Vector3DEditWidget("Mesh Position", false, this);
    meshPositionWidget->setLabelText("X", "Y", "Z");
    meshPositionWidget->setRange(-1000000.0, 1000000.0);
    meshPositionWidget->setSingleStep(1.0);
    meshPositionWidget->setValue(mesh->getLocalPosition());

    meshRotationWidget = new Vector3DEditWidget("Mesh Rotation", true, this);
    meshRotationWidget->setLabelText("X", "Y", "Z");
    meshRotationWidget->setRange(-180, 180);
    meshRotationWidget->setSingleStep(1.0);
    meshRotationWidget->setValue(mesh->getLocalRotation());

    meshScalingWidget = new Vector3DEditWidget("Mesh Scaling", false, this);
    meshScalingWidget->setLabelText("X", "Y", "Z");
    meshScalingWidget->setRange(0.0, 1000000.0);
    meshScalingWidget->setSingleStep(1.0);
    meshScalingWidget->setValue(mesh->getLocalScaling());

    resetTransformationButton = new QPushButton("Reset Transformation", this);
    resetTransformationButton->setFixedWidth(240);

    setupLayout();
    setupSignals();
}

MeshPropertyWidget::~MeshPropertyWidget() {
    delete meshVisibleCheckbox;
    delete meshReverseNormalCheckBox;
    delete meshPositionWidget;
    delete meshRotationWidget;
    delete meshScalingWidget;
    delete resetTransformationButton;
    delete mainLayout;
}

// Private functions

void MeshPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(meshVisibleCheckbox);
    mainLayout->addWidget(meshReverseNormalCheckBox);
    mainLayout->addWidget(meshPositionWidget);
    mainLayout->addWidget(meshRotationWidget);
    mainLayout->addWidget(meshScalingWidget);
    mainLayout->addWidget(resetTransformationButton, 0, Qt::AlignCenter);
    setLayout(mainLayout);
}

void MeshPropertyWidget::setupSignals() {
    connect(meshVisibleCheckbox, SIGNAL(stateChanged(int)), this, SLOT(setMeshVisible(int)));
    connect(meshReverseNormalCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setMeshReverseNormal(int)));
    connect(meshPositionWidget, SIGNAL(valueChanged(QVector3D)), mesh, SLOT(setPosition(QVector3D)));
    connect(meshRotationWidget, SIGNAL(valueChanged(QVector3D)), mesh, SLOT(setRotation(QVector3D)));
    connect(meshScalingWidget, SIGNAL(valueChanged(QVector3D)), mesh, SLOT(setScaling(QVector3D)));
    connect(resetTransformationButton, SIGNAL(clicked(bool)), this, SLOT(resetTransformationButtonClicked()));
}

// Private slots

void MeshPropertyWidget::setMeshVisible(int state) {
    mesh->setVisible(state == Qt::Checked);
}

void MeshPropertyWidget::setMeshReverseNormal(int state) {
    mesh->setReverseNormal(state == Qt::Checked);
}

void MeshPropertyWidget::resetTransformationButtonClicked() {
    mesh->resetTransformation();
    meshPositionWidget->setValue(mesh->getLocalPosition());
    meshRotationWidget->setValue(mesh->getLocalRotation());
    meshScalingWidget->setValue(mesh->getLocalScaling());
}
