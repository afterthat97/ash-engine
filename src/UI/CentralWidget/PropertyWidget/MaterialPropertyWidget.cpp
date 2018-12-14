#include <UI/CentralWidget/PropertyWidget/MaterialPropertyWidget.h>

MaterialPropertyWidget::MaterialPropertyWidget(Material* _material, QWidget *parent): QWidget(parent) {
    material = _material;

    ambientColorWidget = new Vector3DWidget("Ambient Color", true, this);
    ambientColorWidget->setLabelText("R", "G", "B");
    ambientColorWidget->setRange(0.0, 1.0);
    ambientColorWidget->setSingleStep(0.01);
    ambientColorWidget->setValue(material->getAmbientColor());

    diffuseColorWidget = new Vector3DWidget("Diffuse Color", true, this);
    diffuseColorWidget->setLabelText("R", "G", "B");
    diffuseColorWidget->setRange(0.0, 1.0);
    diffuseColorWidget->setSingleStep(0.01);
    diffuseColorWidget->setValue(material->getDiffuseColor());

    specularColorWidget = new Vector3DWidget("Specular Color", true, this);
    specularColorWidget->setLabelText("R", "G", "B");
    specularColorWidget->setRange(0.0, 1.0);
    specularColorWidget->setSingleStep(0.01);
    specularColorWidget->setValue(material->getSpecularColor());

    shininessWidget = new ShininessWidget(this);
    shininessWidget->setShininess(material->getShininess());

    setupLayout();
    setupSignals();
}

MaterialPropertyWidget::~MaterialPropertyWidget() {
    delete ambientColorWidget;
    delete diffuseColorWidget;
    delete specularColorWidget;
    delete shininessWidget;
    delete mainLayout;
}

void MaterialPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(ambientColorWidget);
    mainLayout->addWidget(diffuseColorWidget);
    mainLayout->addWidget(specularColorWidget);
    mainLayout->addWidget(shininessWidget);
    setLayout(mainLayout);
}

void MaterialPropertyWidget::setupSignals() {
    connect(ambientColorWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setAmbientColor(QVector3D)));
    connect(diffuseColorWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setDiffuseColor(QVector3D)));
    connect(specularColorWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setSpecularColor(QVector3D)));
    connect(shininessWidget, SIGNAL(valueChanged(double)), this, SLOT(setShininess(double)));
}

void MaterialPropertyWidget::setAmbientColor(QVector3D value) {
    material->setAmbientColor(value);
}

void MaterialPropertyWidget::setDiffuseColor(QVector3D value) {
    material->setDiffuseColor(value);
}

void MaterialPropertyWidget::setSpecularColor(QVector3D value) {
    material->setSpecularColor(value);
}

void MaterialPropertyWidget::setShininess(double value) {
    material->setShininess(float(value));
}


ShininessWidget::ShininessWidget(QWidget *parent): QGroupBox(parent) {
    setTitle("Shininess");

    label = new QLabel(QStringLiteral("Shininess:"), this);

    spinBox = new QDoubleSpinBox(this);
    spinBox->setRange(0.0, 1000000.0);
    spinBox->setSingleStep(1.0);

    setupLayout();
    setupSignals();
}

ShininessWidget::~ShininessWidget() {
    delete label;
    delete spinBox;
    delete mainLayout;
}

void ShininessWidget::setShininess(float value) {
    spinBox->setValue(value);
}

void ShininessWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(label);
    mainLayout->addWidget(spinBox);
    setLayout(mainLayout);
}

void ShininessWidget::setupSignals() {
    connect(spinBox, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
}
