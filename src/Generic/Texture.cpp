#include <Generic/Texture.h>

Texture::Texture(TextureType _textureType): Object() {
    initID();
    enabled = true;
    image = NULL;
    textureType = _textureType;
}

Texture::~Texture() {
    if (image) delete image;
}

void Texture::setEnabled(bool _enabled) {
    enabled = _enabled;
}

void Texture::setImage(QImage * newImage) {
    if (image) delete image;
    image = newImage;
}

void Texture::setType(TextureType _textureType) {
    textureType = _textureType;
}

bool Texture::isEnabled() {
    return enabled;
}

QImage * Texture::getImage() {
    return image;
}

Texture::TextureType Texture::getType() {
    return textureType;
}

void Texture::initID() {
    id = Allocator::allocateTextureID();
}
