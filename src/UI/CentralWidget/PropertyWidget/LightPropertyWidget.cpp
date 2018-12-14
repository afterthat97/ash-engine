#include <UI/CentralWidget/PropertyWidget/LightPropertyWidget.h>

LightPropertyWidget::LightPropertyWidget(Light* _light, QWidget *parent): QWidget(parent) {
    light = _light;

    lightPositionWidget = new Vector3DWidget("Light Position", false, this);
    lightPositionWidget->setLabelText("X", "Y", "Z");
    lightPositionWidget->setRange(-1000000.0, 1000000.0);
    lightPositionWidget->setSingleStep(1.0);
    lightPositionWidget->setValue(light->getPosition());

    lightColorWidget = new Vector3DWidget("Light Color", true, this);
    lightColorWidget->setLabelText("R", "G", "B");
    lightColorWidget->setRange(0.0, 1.0);
    lightColorWidget->setSingleStep(0.01);
    lightColorWidget->setValue(light->getColor());

    attenuationWidget = new AttenuationWidget(this);
    attenuationWidget->setEnableAttenuation(light->isAttenuationEnabled());
    attenuationWidget->setQuadraticValue(light->getAttenuationQuadratic());
    attenuationWidget->setLinearValue(light->getAttenuationLinear());
    attenuationWidget->setConstantValue(light->getAttenuationConstant());

    setupLayout();
    setupSignals();
}

LightPropertyWidget::~LightPropertyWidget() {
    delete lightPositionWidget;
    delete lightColorWidget;
    delete attenuationWidget;
    delete mainLayout;
}

void LightPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(lightPositionWidget);
    mainLayout->addWidget(lightColorWidget);
    mainLayout->addWidget(attenuationWidget);
    setLayout(mainLayout);
}

void LightPropertyWidget::setupSignals() {
    connect(lightPositionWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setLightPosition(QVector3D)));
    connect(lightColorWidget, SIGNAL(valueChanged(QVector3D)), this, SLOT(setLightColor(QVector3D)));
    connect(attenuationWidget, SIGNAL(enableAttenuationChanged(bool)), this, SLOT(setLightEnableAttenuation(bool)));
    connect(attenuationWidget, SIGNAL(quadraticValueChanged(double)), this, SLOT(setLightAttenuationQuadratic(double)));
    connect(attenuationWidget, SIGNAL(linearValueChanged(double)), this, SLOT(setLightAttenuationLinear(double)));
    connect(attenuationWidget, SIGNAL(constantValueChanged(double)), this, SLOT(setLightAttenuationConstant(double)));
}

void LightPropertyWidget::setLightPosition(QVector3D value) {
    light->setPosition(value);
}

void LightPropertyWidget::setLightColor(QVector3D value) {
    light->setColor(value);
}

void LightPropertyWidget::setLightEnableAttenuation(bool enabled) {
    light->setEnableAttenuation(enabled);
}

void LightPropertyWidget::setLightAttenuationQuadratic(double value) {
    light->setAttenuationQuadratic(float(value));
}

void LightPropertyWidget::setLightAttenuationLinear(double value) {
    light->setAttenuationLinear(float(value));
}

void LightPropertyWidget::setLightAttenuationConstant(double value) {
    light->setAttenuationConstant(float(value));
}


AttenuationWidget::AttenuationWidget(QWidget *parent): QGroupBox(parent) {
    setTitle("Attenuation");

    enableAttenuationCheckBox = new QCheckBox("Enabled", this);

    quadraticLabel = new QLabel("Quadratic value:", this);
    linearLabel = new QLabel("Linear value:", this);
    constantLabel = new QLabel("Constant value:", this);

    quadraticSpinBox = new QDoubleSpinBox(this);
    quadraticSpinBox->setSingleStep(0.000001);
    quadraticSpinBox->setDecimals(6);

    linearSpinBox = new QDoubleSpinBox(this);
    linearSpinBox->setSingleStep(0.0001);
    linearSpinBox->setDecimals(6);

    constantSpinBox = new QDoubleSpinBox(this);
    constantSpinBox->setSingleStep(0.01);
    constantSpinBox->setDecimals(6);

    setupLayout();
    setupSignals();
}

AttenuationWidget::~AttenuationWidget() {
    delete enableAttenuationCheckBox;

    delete quadraticLabel;
    delete linearLabel;
    delete constantLabel;
    
    delete quadraticSpinBox;
    delete linearSpinBox;
    delete constantSpinBox;

    delete mainLayout;
}

void AttenuationWidget::setEnableAttenuation(bool enabled) {
    enableAttenuationCheckBox->setChecked(enabled);
}

void AttenuationWidget::setQuadraticValue(float value) {
    quadraticSpinBox->setValue(value);
}

void AttenuationWidget::setLinearValue(float value) {
    linearSpinBox->setValue(value);
}

void AttenuationWidget::setConstantValue(float value) {
    constantSpinBox->setValue(value);
}

void AttenuationWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(enableAttenuationCheckBox);
    mainLayout->addWidget(quadraticLabel);
    mainLayout->addWidget(quadraticSpinBox);
    mainLayout->addWidget(linearLabel);
    mainLayout->addWidget(linearSpinBox);
    mainLayout->addWidget(constantLabel);
    mainLayout->addWidget(constantSpinBox);
    setLayout(mainLayout);
}

void AttenuationWidget::setupSignals() {
    connect(enableAttenuationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableAttenuationCheckBoxStateChanged(int)));
    connect(quadraticSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(quadraticValueChanged(double)));
    connect(linearSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(linearValueChanged(double)));
    connect(constantSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(constantValueChanged(double)));
}

void AttenuationWidget::enableAttenuationCheckBoxStateChanged(int state) {
    enableAttenuationChanged(state == Qt::Checked);
}
