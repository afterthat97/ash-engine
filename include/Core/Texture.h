#pragma once

#include <Common.h>

class Texture: public QObject {
    Q_OBJECT

public:
    enum TextureType {
        Diffuse = 0,
        Specular = 1,
        Bump = 2
    };
    Texture(TextureType textureType = Diffuse);
    Texture(const Texture& texture);
    ~Texture();

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    bool enabled() const;
    TextureType textureType() const;
    const QImage & image() const;

public slots:
    void setEnabled(bool enabled);
    void setTextureType(TextureType textureType);
    void setImage(const QImage& image);

signals:
    void enabledChanged(bool enabled);
    void textureTypeChanged(TextureType textureType);
    void imageChanged(const QImage& image);

private:
    bool m_enabled;
    TextureType m_textureType;
    QImage m_image;
};

QDataStream &operator>>(QDataStream &in, Texture::TextureType& textureType);
