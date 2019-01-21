#include <UI/PointLightProperty.h>

PointLightProperty::PointLightProperty(PointLight * light, QWidget * parent): QWidget(parent) {
    m_host = light;

    m_enabledCheckBox = new QCheckBox("Enabled", this);
    m_intensityLabel  = new QLabel("Intensity", this);
    m_intensityEdit   = new FloatEdit(0.0f, inf, 2, this);
    m_intensitySlider = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_colorEditSlider = new Vector3DEditSlider("Color", Qt::Horizontal, "R", "G", "B", 0.0f, 1.0f, 2, this);
    m_positionEdit    = new Vector3DEdit("Position", Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);
    m_attenuationEdit = new Vector3DEdit("Attenuation", Qt::Vertical, "Quadratic", "Linear", "Constant", 0.0f, inf, 6, this);

    m_enabledCheckBox->setChecked(m_host->enabled());
    m_intensityEdit->setValue(m_host->intensity());
    m_intensitySlider->setValue(m_host->intensity());
    m_colorEditSlider->setValue(m_host->color());
    m_positionEdit->setValue(m_host->position());
    m_attenuationEdit->setCheckable(true);
    m_attenuationEdit->setChecked(m_host->enableAttenuation());
    m_attenuationEdit->setValue(m_host->attenuationArguments());

    configLayout();
    configSignals();
}

void PointLightProperty::configLayout() {
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_enabledCheckBox, 0, 0);
    mainLayout->addWidget(m_intensityLabel, 1, 0);
    mainLayout->addWidget(m_intensityEdit, 1, 1);
    mainLayout->addWidget(m_intensitySlider, 1, 2);
    mainLayout->addWidget(m_colorEditSlider, 2, 0, 1, 3);
    mainLayout->addWidget(m_positionEdit, 3, 0, 1, 3);
    mainLayout->addWidget(m_attenuationEdit, 4, 0, 1, 3);
    setLayout(mainLayout);
}

void PointLightProperty::configSignals() {
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

    connect(m_enabledCheckBox, SIGNAL(toggled(bool)),          m_host, SLOT(setEnabled(bool)));
    connect(m_intensityEdit,   SIGNAL(valueEdited(float)),     m_host, SLOT(setIntensity(float)));
    connect(m_intensitySlider, SIGNAL(sliderMoved(float)),     m_host, SLOT(setIntensity(float)));
    connect(m_colorEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setColor(QVector3D)));
    connect(m_positionEdit,    SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setPosition(QVector3D)));
    connect(m_attenuationEdit, SIGNAL(toggled(bool)),          m_host, SLOT(setEnableAttenuation(bool)));
    connect(m_attenuationEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setAttenuationArguments(QVector3D)));

    connect(m_host, SIGNAL(enabledChanged(bool)),                   m_enabledCheckBox, SLOT(setChecked(bool)));
    connect(m_host, SIGNAL(intensityChanged(float)),                m_intensityEdit,   SLOT(setValue(float)));
    connect(m_host, SIGNAL(intensityChanged(float)),                m_intensitySlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(colorChanged(QVector3D)),                m_colorEditSlider, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(positionChanged(QVector3D)),             m_positionEdit,    SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(enableAttenuationChanged(bool)),         m_attenuationEdit, SLOT(setChecked(bool)));
    connect(m_host, SIGNAL(attenuationArgumentsChanged(QVector3D)), m_attenuationEdit, SLOT(setValue(QVector3D)));
}

void PointLightProperty::hostDestroyed(QObject *) {
    delete this;
}
