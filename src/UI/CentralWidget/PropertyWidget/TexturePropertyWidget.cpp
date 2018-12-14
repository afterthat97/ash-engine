#include <UI/CentralWidget/PropertyWidget/TexturePropertyWidget.h>

TexturePropertyWidget::TexturePropertyWidget(Texture* _texture, QWidget *parent): QWidget(parent) {
    texture = _texture;

    enableTextureCheckBox = new QCheckBox("Enabled", this);
    enableTextureCheckBox->setChecked(texture->isEnabled());

    typeLabel = new QLabel(this);
    if (texture->getType() == Texture::Diffuse) {
        typeLabel->setText("Texture type: Diffuse map");
    } else if (texture->getType() == Texture::Specular) {
        typeLabel->setText("Texture type: Specular map");
    } else if (texture->getType() == Texture::Normal) {
        typeLabel->setText("Texture type: Normal map");
    }

    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap::fromImage(*texture->getImage()).scaled(300, 300, Qt::KeepAspectRatio));

    setupLayout();
    setupSignals();
}

TexturePropertyWidget::~TexturePropertyWidget() {
    delete enableTextureCheckBox;
    delete typeLabel;
    delete imageLabel;
    delete mainLayout;
}

void TexturePropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(enableTextureCheckBox);
    mainLayout->addWidget(typeLabel);
    mainLayout->addWidget(imageLabel);
    setLayout(mainLayout);
}

void TexturePropertyWidget::setupSignals() {
    connect(enableTextureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setTextureEnabled(int)));
}

void TexturePropertyWidget::setTextureEnabled(int state) {
    texture->setEnabled(state == Qt::Checked);
}
