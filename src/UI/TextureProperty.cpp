#include <UI/TextureProperty.h>

TexturePropertyWidget::TexturePropertyWidget(Texture * texture, QWidget * parent): QWidget(parent) {
    m_host = texture;

    m_enabledCheckBox  = new QCheckBox("Enabled", this);
    m_textureTypeLabel = new QLabel(this);
    m_filePathLabel    = new QLabel(this);
    m_imageLabel       = new QLabel(this);

    m_enabledCheckBox->setChecked(m_host->enabled());
    if (texture->textureType() == Texture::Diffuse) {
        m_textureTypeLabel->setText("Type: Diffuse map");
    } else if (texture->textureType() == Texture::Specular) {
        m_textureTypeLabel->setText("Type: Specular map");
    } else if (texture->textureType() == Texture::Bump) {
        m_textureTypeLabel->setText("Type: Bump map");
    }
    m_filePathLabel->setText("Path: " + m_host->filePath());
    m_imageLabel->setPixmap(QPixmap::fromImage(texture->image()).scaled(300, 300, Qt::KeepAspectRatio));

    configLayout();
    configSignals();
}

void TexturePropertyWidget::configLayout() {
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_enabledCheckBox);
    mainLayout->addWidget(m_textureTypeLabel);
    mainLayout->addWidget(m_filePathLabel);
    mainLayout->addWidget(m_imageLabel);
    setLayout(mainLayout);
}

void TexturePropertyWidget::configSignals() {
    connect(m_enabledCheckBox, SIGNAL(toggled(bool)), m_host, SLOT(setEnabled(bool)));
    connect(m_host, SIGNAL(enabledChanged(bool)), m_enabledCheckBox, SLOT(setChecked(bool)));
}
