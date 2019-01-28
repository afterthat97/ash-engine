#include <Core/Texture.h>
#include <qimagereader.h>

Texture::Texture(TextureType textureType) : QObject(0) {
    m_enabled = true;
    m_textureType = textureType;
}

Texture::Texture(const Texture & texture) {
    m_enabled = texture.m_enabled;
    m_textureType = texture.m_textureType;
    m_image = texture.m_image;
}

Texture::~Texture() {
#ifdef _DEBUG
    qDebug() << "Texture" << this->objectName() << "is destroyed";
#endif
}

void Texture::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Texture: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Enabled: " << m_enabled;
    qDebug().nospace() << tab(l + 1) << "Type: " <<
        (m_textureType == Diffuse ? "Diffuse" : (m_textureType == Specular ? "Specular" : "Height"));
    qDebug().nospace() << tab(l + 1) << "Resolution: " << m_image.width() << "*" << m_image.height();
}

void Texture::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

bool Texture::enabled() const {
    return m_enabled;
}

Texture::TextureType Texture::textureType() const {
    return m_textureType;
}

const QImage & Texture::image() const {
    return m_image;
}

void Texture::setEnabled(bool enabled) {
    if (m_enabled != enabled) {
        m_enabled = enabled;
        enabledChanged(m_enabled);
    }
}

void Texture::setTextureType(TextureType textureType) {
    if (m_textureType != textureType) {
        m_textureType = textureType;
        textureTypeChanged(m_textureType);
    }
}

void Texture::setImage(const QImage & image) {
    if (m_image != image) {
        m_image = image;
        imageChanged(m_image);
    }
}

QDataStream & operator>>(QDataStream & in, Texture::TextureType & textureType) {
    qint32 t;
    in >> t;
    if (t == 0)
        textureType = Texture::Diffuse;
    else if (t == 1)
        textureType = Texture::Specular;
    else
        textureType = Texture::Bump;
    return in;
}
