#pragma once

#include <Generic/Texture.h>

class Material: public QObject {
    Q_OBJECT

public:
    Material(QObject * parent = 0);
    ~Material();

    vector<Texture*> getTextures();
    QVector3D getAmbientColor();
    QVector3D getDiffuseColor();
    QVector3D getSpecularColor();
    float getShininess();

public slots:
    void addTexture(Texture* newTexture);
    void setAmbientColor(QVector3D color);
    void setDiffuseColor(QVector3D color);
    void setSpecularColor(QVector3D color);
    void setShininess(float value);

private:
    QVector3D ambientColor, diffuseColor, specularColor;
    float shininess;
    vector<Texture*> textures;
};