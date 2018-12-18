#include <UI/CentralWidget/PropertyWidget/ModelPropertyWidget.h>

ModelPropertyWidget::ModelPropertyWidget(Model* _model, QWidget *parent): QWidget(parent) {
    model = _model;

    modelVisibleCheckbox = new QCheckBox("Visible", this);
    modelVisibleCheckbox->setChecked(model->isVisible());

    modelPositionWidget = new Vector3DEditWidget("Model Position", false, this);
    modelPositionWidget->setLabelText("X", "Y", "Z");
    modelPositionWidget->setRange(-1000000.0, 1000000.0);
    modelPositionWidget->setSingleStep(1.0);
    modelPositionWidget->setValue(model->getLocalPosition());

    modelRotationWidget = new Vector3DEditWidget("Model Rotation", true, this);
    modelRotationWidget->setLabelText("X", "Y", "Z");
    modelRotationWidget->setRange(-180, 180);
    modelRotationWidget->setSingleStep(1.0);
    modelRotationWidget->setValue(model->getLocalRotation());

    modelScalingWidget = new Vector3DEditWidget("Model Scaling", false, this);
    modelScalingWidget->setLabelText("X", "Y", "Z");
    modelScalingWidget->setRange(0.0, 1000000.0);
    modelScalingWidget->setSingleStep(1.0);
    modelScalingWidget->setValue(model->getLocalScaling());

    resetTransformationButton = new QPushButton("Reset Transformation", this);
    resetTransformationButton->setFixedWidth(240);
    resetChildrenTransformationButton = new QPushButton("Reset Children's Tranformation", this);
    resetChildrenTransformationButton->setFixedWidth(240);

    setupLayout();
    setupSignals();
}

ModelPropertyWidget::~ModelPropertyWidget() {
    delete modelVisibleCheckbox;
    delete modelPositionWidget;
    delete modelRotationWidget;
    delete modelScalingWidget;
    delete resetTransformationButton;
    delete resetChildrenTransformationButton;
    delete mainLayout;
}

// Private functions

void ModelPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(modelVisibleCheckbox);
    mainLayout->addWidget(modelPositionWidget);
    mainLayout->addWidget(modelRotationWidget);
    mainLayout->addWidget(modelScalingWidget);
    mainLayout->addWidget(resetTransformationButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(resetChildrenTransformationButton, 0, Qt::AlignCenter);
    setLayout(mainLayout);
}

void ModelPropertyWidget::setupSignals() {
    connect(modelVisibleCheckbox, SIGNAL(stateChanged(int)), this, SLOT(setModelVisible(int)));
    connect(modelPositionWidget, SIGNAL(valueChanged(QVector3D)), model, SLOT(setPosition(QVector3D)));
    connect(modelRotationWidget, SIGNAL(valueChanged(QVector3D)), model, SLOT(setRotation(QVector3D)));
    connect(modelScalingWidget, SIGNAL(valueChanged(QVector3D)), model, SLOT(setScaling(QVector3D)));
    connect(resetTransformationButton, SIGNAL(clicked(bool)), this, SLOT(resetTransformationButtonClicked()));
    connect(resetChildrenTransformationButton, SIGNAL(clicked(bool)), this, SLOT(resetChildrenTransformationButtonClicked()));
}

// Private slots

void ModelPropertyWidget::setModelVisible(int state) {
    model->setVisible(state == Qt::Checked);
}

void ModelPropertyWidget::resetTransformationButtonClicked() {
    model->resetTransformation();
    modelPositionWidget->setValue(model->getLocalPosition());
    modelRotationWidget->setValue(model->getLocalRotation());
    modelScalingWidget->setValue(model->getLocalScaling());
}

void ModelPropertyWidget::resetChildrenTransformationButtonClicked() {
    model->resetChildrenTransformation();
}