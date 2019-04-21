#include <Material.h>

Material::Material(QObject* parent): QObject(0) {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    m_ambient = 0.2f;
    m_diffuse = 1.0f;
    m_specular = 0.5f;
    m_shininess = 32.0f;
    setObjectName("Untitled Material");
    setParent(parent);
}

Material::Material(QVector3D color,
                   float ambient, float diffuse, float specular,
                   QObject * parent): QObject(0) {
    m_color = color;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = 32.0f;
    setObjectName("Untitled Material");
    setParent(parent);
}

Material::Material(const Material & material): QObject(0) {
    m_color = material.m_color;
    m_ambient = material.m_ambient;
    m_diffuse = material.m_diffuse;
    m_specular = material.m_specular;
    m_shininess = material.m_shininess;
    m_diffuseTexture = material.m_diffuseTexture;
    m_specularTexture = material.m_specularTexture;
    m_bumpTexture = material.m_bumpTexture;
    setObjectName(material.objectName());
}

Material::~Material() {
    if (log_level >= LOG_LEVEL_INFO)
        dout << "Material" << this->objectName() << "is destroyed";
}

void Material::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Material: " << objectName();
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
    if (!isEqual(m_color, color)) {
        m_color = color;
        if (log_level >= LOG_LEVEL_INFO)
            dout << "The color of" << this->objectName() << "is set to" << color;
        colorChanged(m_color);
    }
}

void Material::setAmbient(float ambient) {
    if (!isEqual(m_ambient, ambient)) {
        m_ambient = ambient;
        if (log_level >= LOG_LEVEL_INFO)
            dout << "The ambient weight of" << this->objectName() << "is set to" << ambient;
        ambientChanged(m_ambient);
    }
}

void Material::setDiffuse(float diffuse) {
    if (!isEqual(m_diffuse, diffuse)) {
        m_diffuse = diffuse;
        if (log_level >= LOG_LEVEL_INFO)
            dout << "The diffuse weight of" << this->objectName() << "is set to" << diffuse;
        diffuseChanged(m_diffuse);
    }
}

void Material::setSpecular(float specular) {
    if (!isEqual(m_specular, specular)) {
        m_specular = specular;
        if (log_level >= LOG_LEVEL_INFO)
            dout << "The specular weight of" << this->objectName() << "is set to" << specular;
        specularChanged(m_specular);
    }
}

void Material::setShininess(float shininess) {
    if (!isEqual(m_shininess, shininess)) {
        m_shininess = shininess;
        if (log_level >= LOG_LEVEL_INFO)
            dout << "The shininess of" << this->objectName() << "is set to" << shininess;
        shininessChanged(m_shininess);
    }
}

void Material::setDiffuseTexture(QSharedPointer<Texture> diffuseTexture) {
    if (m_diffuseTexture != diffuseTexture) {
        m_diffuseTexture = diffuseTexture;
        if (log_level >= LOG_LEVEL_INFO && !diffuseTexture.isNull())
            dout << "Diffuse texture" << diffuseTexture->objectName()
                 << "is assigned to material" << this->objectName();
        diffuseTextureChanged(m_diffuseTexture);
    }
}

void Material::setSpecularTexture(QSharedPointer<Texture> specularTexture) {
    if (m_specularTexture != specularTexture) {
        m_specularTexture = specularTexture;
        if (log_level >= LOG_LEVEL_INFO && !specularTexture.isNull())
            dout << "Specular texture" << specularTexture->objectName()
                 << "is assigned to material" << this->objectName();
        specularTextureChanged(m_specularTexture);
    }
}

void Material::setBumpTexture(QSharedPointer<Texture> bumpTexture) {
    if (m_bumpTexture != bumpTexture) {
        m_bumpTexture = bumpTexture;
        if (log_level >= LOG_LEVEL_INFO && !bumpTexture.isNull())
            dout << "Bump texture" << bumpTexture->objectName()
                 << "is assigned to material" << this->objectName();
        bumpTextureChanged(m_bumpTexture);
    }
}
