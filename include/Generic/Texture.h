#pragma once

#include <Generic/Allocator.h>

class Texture: public QObject {
    Q_OBJECT

public:
    enum TextureType {
        Diffuse,
        Specular,
        Normal
    };
    Texture(TextureType textureType, QObject * parent = 0);
    ~Texture();

    uint32_t getID();
    bool isEnabled();
    QImage* getImage();
    TextureType getType();

public slots:
    void setEnabled(bool enabled);
    void setImage(QImage* newImage);
    void setType(TextureType textureType);

private:
    uint32_t id;
    bool enabled;
    QImage *image;
    TextureType textureType;
};
