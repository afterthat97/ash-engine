#include <UI/MaterialProperty.h>

MaterialProperty::MaterialProperty(Material * material, QWidget * parent): QWidget(parent) {
    m_host = material;

    m_ambientLabel   = new QLabel("Ambient:",   this);
    m_diffuseLabel   = new QLabel("Diffuse:",   this);
    m_specularLabel  = new QLabel("Specular:",  this);
    m_shininessLabel = new QLabel("Shininess:", this);

    m_ambientEdit   = new FloatEdit(0.0f, inf, 2, this);
    m_diffuseEdit   = new FloatEdit(0.0f, inf, 2, this);
    m_specularEdit  = new FloatEdit(0.0f, inf, 2, this);
    m_shininessEdit = new FloatEdit(0.0f, inf, 2, this);

    m_ambientSlider   = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_diffuseSlider   = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_specularSlider  = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_shininessSlider = new FloatSlider(Qt::Horizontal, 0.0f, 100.0f,  this);

    m_colorEditSlider = new Vector3DEditSlider("Color", Qt::Horizontal, "R", "G", "B", 0.0f, 1.0f, 2, this);

    m_ambientEdit->setValue(m_host->ambient());
    m_diffuseEdit->setValue(m_host->diffuse());
    m_specularEdit->setValue(m_host->specular());
    m_shininessEdit->setValue(m_host->shininess());

    m_ambientSlider->setValue(m_host->ambient());
    m_diffuseSlider->setValue(m_host->diffuse());
    m_specularSlider->setValue(m_host->specular());
    m_shininessSlider->setValue(m_host->shininess());

    m_colorEditSlider->setValue(m_host->color());

    configLayout();
    configSignals();
}

void MaterialProperty::configLayout() {
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_ambientLabel, 0, 0);
    mainLayout->addWidget(m_ambientEdit, 0, 1);
    mainLayout->addWidget(m_ambientSlider, 0, 2);
    mainLayout->addWidget(m_diffuseLabel, 1, 0);
    mainLayout->addWidget(m_diffuseEdit, 1, 1);
    mainLayout->addWidget(m_diffuseSlider, 1, 2);
    mainLayout->addWidget(m_specularLabel, 2, 0);
    mainLayout->addWidget(m_specularEdit, 2, 1);
    mainLayout->addWidget(m_specularSlider, 2, 2);
    mainLayout->addWidget(m_shininessLabel, 3, 0);
    mainLayout->addWidget(m_shininessEdit, 3, 1);
    mainLayout->addWidget(m_shininessSlider, 3, 2);
    mainLayout->addWidget(m_colorEditSlider, 4, 0, 1, 3);
    setLayout(mainLayout);
}

void MaterialProperty::configSignals() {
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

    connect(m_ambientEdit, SIGNAL(valueEdited(float)), m_host, SLOT(setAmbient(float)));
    connect(m_diffuseEdit, SIGNAL(valueEdited(float)), m_host, SLOT(setDiffuse(float)));
    connect(m_specularEdit, SIGNAL(valueEdited(float)), m_host, SLOT(setSpecular(float)));
    connect(m_shininessEdit, SIGNAL(valueEdited(float)), m_host, SLOT(setShininess(float)));
    connect(m_ambientSlider, SIGNAL(sliderMoved(float)), m_host, SLOT(setAmbient(float)));
    connect(m_diffuseSlider, SIGNAL(sliderMoved(float)), m_host, SLOT(setDiffuse(float)));
    connect(m_specularSlider, SIGNAL(sliderMoved(float)), m_host, SLOT(setSpecular(float)));
    connect(m_shininessSlider, SIGNAL(sliderMoved(float)), m_host, SLOT(setShininess(float)));
    connect(m_colorEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setColor(QVector3D)));

    connect(m_host, SIGNAL(ambientChanged(float)), m_ambientEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(diffuseChanged(float)), m_diffuseEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(specularChanged(float)), m_specularEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(shininessChanged(float)), m_shininessEdit, SLOT(setValue(float)));
    connect(m_host, SIGNAL(ambientChanged(float)), m_ambientSlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(diffuseChanged(float)), m_diffuseSlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(specularChanged(float)), m_specularSlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(shininessChanged(float)), m_shininessSlider, SLOT(setValue(float)));
    connect(m_host, SIGNAL(colorChanged(QVector3D)), m_colorEditSlider, SLOT(setValue(QVector3D)));
}

void MaterialProperty::hostDestroyed(QObject *) {
    delete this;
}
