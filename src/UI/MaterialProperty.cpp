#include <MaterialProperty.h>

MaterialProperty::MaterialProperty(Material * material, QWidget * parent): QWidget(parent) {
    m_host = material;

    m_ambientLabel   = new QLabel("Ambient:",   this);
    m_diffuseLabel   = new QLabel("Diffuse:",   this);
    m_specularLabel  = new QLabel("Specular:",  this);
    m_shininessLabel = new QLabel("Shininess:", this);

    m_ambientEdit   = new FloatEdit(0.0f, inf, 2, this);
    m_diffuseEdit   = new FloatEdit(0.0f, inf, 2, this);
    m_specularEdit  = new FloatEdit(0.0f, inf, 2, this);
    m_shininessEdit = new FloatEdit(0.01f, inf, 2, this);

    m_ambientSlider   = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_diffuseSlider   = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_specularSlider  = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
    m_shininessSlider = new FloatSlider(Qt::Horizontal, 0.01f, 100.0f,  this);

    m_colorEditSlider = new Vector3DEditSlider("Color", Qt::Horizontal, "R", "G", "B", 0.0f, 1.0f, 2, this);

    if (m_host->diffuseTexture())
        m_diffuseMapProperty = new TextureProperty(m_host->diffuseTexture().data(), this);
    else
        m_diffuseMapProperty = 0;

    if (m_host->specularTexture())
        m_specularMapProperty = new TextureProperty(m_host->specularTexture().data(), this);
    else
        m_specularMapProperty = 0;

    if (m_host->bumpTexture())
        m_bumpMapProperty = new TextureProperty(m_host->bumpTexture().data(), this);
    else
        m_bumpMapProperty = 0;

    m_ambientEdit->setValue(m_host->ambient());
    m_diffuseEdit->setValue(m_host->diffuse());
    m_specularEdit->setValue(m_host->specular());
    m_shininessEdit->setValue(m_host->shininess());

    m_ambientSlider->setValue(m_host->ambient());
    m_diffuseSlider->setValue(m_host->diffuse());
    m_specularSlider->setValue(m_host->specular());
    m_shininessSlider->setValue(m_host->shininess());

    m_colorEditSlider->setValue(m_host->color());

    if (m_diffuseMapProperty)
        m_colorEditSlider->setDisabled(m_diffuseMapProperty->isChecked());

    if (m_specularMapProperty) {
        m_specularEdit->setDisabled(m_specularMapProperty->isChecked());
        m_specularSlider->setDisabled(m_specularMapProperty->isChecked());
    }

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
    if (m_diffuseMapProperty)
        mainLayout->addWidget(m_diffuseMapProperty, 5, 0, 1, 3);
    if (m_specularMapProperty)
        mainLayout->addWidget(m_specularMapProperty, 6, 0, 1, 3);
    if (m_bumpMapProperty)
        mainLayout->addWidget(m_bumpMapProperty, 7, 0, 1, 3);
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
    if (m_diffuseMapProperty)
        connect(m_diffuseMapProperty, SIGNAL(toggled(bool)), m_colorEditSlider, SLOT(setDisabled(bool)));
    if (m_specularMapProperty) {
        connect(m_specularMapProperty, SIGNAL(toggled(bool)), m_specularEdit, SLOT(setDisabled(bool)));
        connect(m_specularMapProperty, SIGNAL(toggled(bool)), m_specularSlider, SLOT(setDisabled(bool)));
    }

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

TextureProperty::TextureProperty(Texture * texture, QWidget * parent): QGroupBox(parent) {
    m_host = texture;

    if (m_host->textureType() == Texture::Diffuse)
        setTitle("Diffuse Map");
    else if (m_host->textureType() == Texture::Specular)
        setTitle("Specular Map");
    else
        setTitle("Bump(Normal) map");

    setCheckable(true);
    setChecked(m_host->enabled());

    m_widthTextLabel = new QLabel("Width:", this);
    m_widthValueLabel = new QLabel(QString::number(m_host->image().mirrored().width()), this);
    m_heightTextLabel = new QLabel("Height:", this);
    m_heightValueLabel = new QLabel(QString::number(m_host->image().mirrored().height()), this);

    m_imageLabel = new QLabel(this);
    m_imageLabel->setMinimumWidth(1);

    configLayout();
    configSignals();
}

void TextureProperty::resizeEvent(QResizeEvent * e) {
    QImage image = m_host->image().mirrored();
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaledToWidth(qMin(int(m_imageLabel->width() * devicePixelRatioF()), image.width()));
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    m_imageLabel->setPixmap(pixmap);
    e->accept();
}

void TextureProperty::configLayout() {
    QGridLayout * subLayout = new QGridLayout;
    subLayout->addWidget(m_widthTextLabel, 0, 0);
    subLayout->addWidget(m_widthValueLabel, 0, 1);
    subLayout->addWidget(m_heightTextLabel, 1, 0);
    subLayout->addWidget(m_heightValueLabel, 1, 1);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(subLayout);
    mainLayout->addWidget(m_imageLabel);
    setLayout(mainLayout);
}

void TextureProperty::configSignals() {
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
    connect(m_host, SIGNAL(enabledChanged(bool)), this, SLOT(setChecked(bool)));
    connect(m_host, SIGNAL(textureTypeChanged(Texture::TextureType)), this, SLOT(textureTypeChanged(Texture::TextureType)));
    connect(m_host, SIGNAL(imageChanged(const QImage &)), this, SLOT(imageChanged(const QImage &)));

    connect(this, SIGNAL(toggled(bool)), m_host, SLOT(setEnabled(bool)));
}

void TextureProperty::hostDestroyed(QObject *) {
    delete this;
}

void TextureProperty::textureTypeChanged(Texture::TextureType textureType) {
    if (textureType == Texture::Diffuse)
        setTitle("Diffuse Map");
    else if (textureType == Texture::Specular)
        setTitle("Specular Map");
    else
        setTitle("Bump(Normal) map");
}

void TextureProperty::imageChanged(const QImage & image) {
    m_widthValueLabel->setText(QString::number(image.mirrored().width()));
    m_heightValueLabel->setText(QString::number(image.mirrored().height()));
}
