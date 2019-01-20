#include <UI/DirectionalLightProperty.h>

DirectionalLightProperty::DirectionalLightProperty(DirectionalLight * light, QWidget * parent): QWidget(parent) {
    m_host = light;

    m_enabledCheckBox = new QCheckBox("Enabled", this);
    m_intensityLabel  = new QLabel("Intensity", this);
    m_intensityEdit   = new FloatEdit(0.0f, inf, 2, this);
    m_intensitySlider = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_colorEditSlider = new Vector3DEditSlider("Color", Qt::Horizontal, "R", "G", "B", 0.0f, 1.0f, 2, this);
    m_directionEdit   = new Vector3DEdit("Position", Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);

    m_enabledCheckBox->setChecked(m_host->enabled());
    m_intensityEdit->setValue(m_host->intensity());
    m_intensitySlider->setValue(m_host->intensity());
    m_colorEditSlider->setValue(m_host->color());
    m_directionEdit->setValue(m_host->direction());

    configLayout();
    configSignals();
}

void DirectionalLightProperty::configLayout() {
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_enabledCheckBox, 0, 0);
    mainLayout->addWidget(m_intensityLabel, 1, 0);
    mainLayout->addWidget(m_intensityEdit, 1, 1);
    mainLayout->addWidget(m_intensitySlider, 1, 2);
    mainLayout->addWidget(m_colorEditSlider, 2, 0, 1, 3);
    mainLayout->addWidget(m_directionEdit, 3, 0, 1, 3);
    setLayout(mainLayout);
}

void DirectionalLightProperty::configSignals() {
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

    connect(m_enabledCheckBox, SIGNAL(toggled(bool)), m_host, SLOT(setEnabled(bool)));
    connect(m_intensityEdit, SIGNAL(valueEdited(float)), m_host, SLOT(setIntensity(float)));
    connect(m_intensitySlider, SIGNAL(valueSlided(float)), m_host, SLOT(setIntensity(float)));
    connect(m_colorEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setColor(QVector3D)));
    connect(m_directionEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setDirection(QVector3D)));

    connect(m_host, SIGNAL(enabledChanged(bool)), m_enabledCheckBox, SLOT(setChecked(bool)));
    connect(m_host, SIGNAL(intensityChanged(float)), m_intensityEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(intensityChanged(float)), m_intensitySlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(colorChanged(QVector3D)), m_colorEditSlider, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(directionChanged(QVector3D)), m_directionEdit, SLOT(setValue(QVector3D)));
}

void DirectionalLightProperty::hostDestroyed(QObject *) {
    delete this;
}
