#pragma once

#include <Generic/Texture.h>

class Material: public Object {
public:
    Material();
    ~Material();

    void addTexture(Texture* newTexture);
    void setAmbientColor(QVector3D color);
    void setDiffuseColor(QVector3D color);
    void setSpecularColor(QVector3D color);
    void setShininess(float value);
    
    vector<Texture*> getTextures();
    QVector3D getAmbientColor();
    QVector3D getDiffuseColor();
    QVector3D getSpecularColor();
    float getShininess();

private:
    QVector3D ambientColor, diffuseColor, specularColor;
    float shininess;
    vector<Texture*> textures;

    void initName() override;
    void initID() override;
};