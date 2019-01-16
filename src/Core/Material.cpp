#include <Core/Material.h>

Material::Material(QVector3D color, QObject * parent): QObject(parent) {
    m_color = color;
    m_ambient = 0.2f;
    m_diffuse = 1.0f;
    m_specular = 0.5f;
    m_shininess = 32.0f;
}

void Material::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Phong Material: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Color: " << m_color;
    qDebug().nospace() << tab(l + 1) << "Ambient Intensity: " << m_ambient;
    qDebug().nospace() << tab(l + 1) << "Diffuse Intensity: " << m_diffuse;
    qDebug().nospace() << tab(l + 1) << "Specular Intensity: " << m_specular;
    qDebug().nospace() << tab(l + 1) << "Shininess: " << m_shininess;
}

void Material::dumpObjectTree(int l) {
    dumpObjectInfo(l);
    if (!m_diffuseTexture.isNull())
        m_diffuseTexture->dumpObjectTree(l + 1);
    if (!m_specularTexture.isNull())
        m_specularTexture->dumpObjectTree(l + 1);
    if (!m_bumpTexture.isNull())
        m_bumpTexture->dumpObjectTree(l + 1);
}

QVector3D Material::color() const {
    return m_color;
}

float Material::ambient() {
    return m_ambient;
}

float Material::diffuse() {
    return m_diffuse;
}

float Material::specular() {
    return m_specular;
}

float Material::shininess() {
    return m_shininess;
}

QSharedPointer<Texture> Material::diffuseTexture() {
    return m_diffuseTexture;
}

QSharedPointer<Texture> Material::specularTexture() {
    return m_specularTexture;
}

QSharedPointer<Texture> Material::bumpTexture() {
    return m_bumpTexture;
}

void Material::setColor(QVector3D color) {
    if (!qFuzzyCompare(m_color, color)) {
        m_color = color;
        colorChanged(m_color);
    }
}

void Material::setAmbient(float ambient) {
    if (!qFuzzyCompare(m_ambient, ambient)) {
        m_ambient = ambient;
        ambientChanged(m_ambient);
    }
}

void Material::setDiffuse(float diffuse) {
    if (!qFuzzyCompare(m_diffuse, diffuse)) {
        m_diffuse = diffuse;
        diffuseChanged(m_diffuse);
    }
}

void Material::setSpecular(float specular) {
    if (!qFuzzyCompare(m_specular, specular)) {
        m_specular = specular;
        specularChanged(m_specular);
    }
}

void Material::setShininess(float shininess) {
    if (!qFuzzyCompare(m_shininess, shininess)) {
        m_shininess = shininess;
        shininessChanged(m_shininess);
    }
}

void Material::setDiffuseTexture(QSharedPointer<Texture> diffuseTexture) {
    if (m_diffuseTexture != diffuseTexture) {
        m_diffuseTexture = diffuseTexture;
        diffuseTextureChanged(m_diffuseTexture);
    }
}

void Material::setSpecularTexture(QSharedPointer<Texture> specularTexture) {
    if (m_specularTexture != specularTexture) {
        m_specularTexture = specularTexture;
        specularTextureChanged(m_specularTexture);
    }
}

void Material::setBumpTexture(QSharedPointer<Texture> bumpTexture) {
    if (m_bumpTexture != bumpTexture) {
        m_bumpTexture = bumpTexture;
        bumpTextureChanged(m_bumpTexture);
    }
}
