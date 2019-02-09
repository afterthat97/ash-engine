#pragma once

#include <Texture.h>

class Material: public QObject {
    Q_OBJECT

public:
    Material(QObject * parent = 0);
    Material(QVector3D color,
             float ambient, float diffuse, float specular,
             QObject * parent = 0);
    Material(const Material& material);
    ~Material();

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    QVector3D color() const;
    float ambient();
    float diffuse();
    float specular();
    float shininess();
    QSharedPointer<Texture> diffuseTexture();
    QSharedPointer<Texture> specularTexture();
    QSharedPointer<Texture> bumpTexture();

public slots:
    void setColor(QVector3D color);
    void setAmbient(float ambient);
    void setDiffuse(float diffuse);
    void setSpecular(float specular);
    void setShininess(float shininess);
    void setDiffuseTexture(QSharedPointer<Texture> diffuseTexture);
    void setSpecularTexture(QSharedPointer<Texture> specularTexture);
    void setBumpTexture(QSharedPointer<Texture> bumpTexture);

signals:
    void colorChanged(QVector3D color);
    void ambientChanged(float ambient);
    void diffuseChanged(float diffuse);
    void specularChanged(float specular);
    void shininessChanged(float shininess);
    void diffuseTextureChanged(QSharedPointer<Texture> diffuseTexture);
    void specularTextureChanged(QSharedPointer<Texture> specularTexture);
    void bumpTextureChanged(QSharedPointer<Texture> bumpTexture);

protected:
    QVector3D m_color;
    float m_ambient, m_diffuse, m_specular, m_shininess;
    QSharedPointer<Texture> m_diffuseTexture, m_specularTexture, m_bumpTexture;
};
