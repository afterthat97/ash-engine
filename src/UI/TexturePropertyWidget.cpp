#include <UI/TexturePropertyWidget.h>

TexturePropertyWidget::TexturePropertyWidget(Texture* _texture, QWidget *parent): QWidget(parent) {
    texture = _texture;

    enableTextureCheckBox = new QCheckBox(QStringLiteral("启用贴图"), this);
    enableTextureCheckBox->setChecked(texture->isEnabled());

    typeLabel = new QLabel(this);
    if (texture->getType() == Texture::Diffuse) {
        typeLabel->setText(QStringLiteral("贴图类型：漫反射贴图"));
    } else if (texture->getType() == Texture::Specular) {
        typeLabel->setText(QStringLiteral("贴图类型：镜面反射贴图"));
    } else if (texture->getType() == Texture::Normal) {
        typeLabel->setText(QStringLiteral("贴图类型：法线贴图"));
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
}

void TexturePropertyWidget::setupLayout() {
    QVBoxLayout * mainLayout = new QVBoxLayout;
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
