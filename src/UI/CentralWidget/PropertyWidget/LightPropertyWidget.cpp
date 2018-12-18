#include <UI/CentralWidget/PropertyWidget/LightPropertyWidget.h>

// LightPropertyWidget constructor

LightPropertyWidget::LightPropertyWidget(Light* _light, QWidget *parent): QWidget(parent) {
    light = _light;

    positionEditWidget = new Vector3DEditWidget("Light Position", false, this);
    positionEditWidget->setLabelText("X", "Y", "Z");
    positionEditWidget->setRange(-1000000.0, 1000000.0);
    positionEditWidget->setSingleStep(1.0);
    positionEditWidget->setValue(light->getPosition());

    colorEditWidget = new Vector3DEditWidget("Light Color", true, this);
    colorEditWidget->setLabelText("R", "G", "B");
    colorEditWidget->setRange(0.0, 1.0);
    colorEditWidget->setSingleStep(0.01);
    colorEditWidget->setValue(light->getColor());

    attenuationEditWidget = new Vector3DEditWidget("Light Attenuation", false, this);
    attenuationEditWidget->setLabelText("Quadratic", "Linear", "Constant");
    attenuationEditWidget->setRange(0.0, 1000000.0);
    attenuationEditWidget->setSingleStep(0.000001);
    attenuationEditWidget->setDecimals(6);
    attenuationEditWidget->setValue(QVector3D(light->getAttenuationQuadratic(),
                                              light->getAttenuationLinear(),
                                              light->getAttenuationConstant()));

    enableAttenuationCheckBox = new QCheckBox("Enabled", attenuationEditWidget);
    enableAttenuationCheckBox->setChecked(light->isAttenuationEnabled());
    attenuationEditWidget->insertWidget(enableAttenuationCheckBox, 0);

    setupLayout();
    setupSignals();
}

LightPropertyWidget::~LightPropertyWidget() {
    delete positionEditWidget;
    delete colorEditWidget;
    delete enableAttenuationCheckBox;
    delete attenuationEditWidget;
    delete mainLayout;
}

// Private functions

void LightPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(positionEditWidget);
    mainLayout->addWidget(colorEditWidget);
    mainLayout->addWidget(attenuationEditWidget);
    setLayout(mainLayout);
}

void LightPropertyWidget::setupSignals() {
    connect(positionEditWidget, SIGNAL(valueChanged(QVector3D)), light, SLOT(setPosition(QVector3D)));
    connect(colorEditWidget, SIGNAL(valueChanged(QVector3D)), light, SLOT(setColor(QVector3D)));
    connect(attenuationEditWidget, SIGNAL(valueChanged(QVector3D)), light, SLOT(setAttenuationValue(QVector3D)));
    connect(enableAttenuationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setEnableAttenuation(int)));
}

// Private slots

void LightPropertyWidget::setEnableAttenuation(int state) {
    light->setEnableAttenuation(state == Qt::Checked);
}