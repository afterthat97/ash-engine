#pragma once

#include <Generic/Object.h>
#include <Generic/Allocator.h>

class Texture: public Object {
public:
    enum TextureType {
        Diffuse,
        Specular,
        Normal
    };
    Texture(TextureType textureType);
    ~Texture();

    void setEnabled(bool enabled);
    void setImage(QImage* newImage);
    void setType(TextureType textureType);

    bool isEnabled();
    QImage* getImage();
    TextureType getType();

private:
    bool enabled;
    QImage *image;
    TextureType textureType;

    void initName() override;
    void initID() override;
};
