#include <OpenGLMaterial.h>
#include <OpenGLTexture.h>

struct ShaderMaterialInfo {
    QVector4D color;      // 16          // 0
    float ambient;        // 4           // 16
    float diffuse;        // 4           // 20
    float specular;       // 4           // 24
    float shininess;      // 4           // 28
    int useDiffuseMap;    // 4           // 32
    int useSpecularMap;   // 4           // 36
    int useBumpMap;       // 4           // 40
    int padding;          // 4           // 44
} shaderMaterialInfo;

OpenGLUniformBufferObject *OpenGLMaterial::m_materialInfo = 0;

OpenGLMaterial::OpenGLMaterial(Material * material, QObject* parent): QObject(0) {
    m_host = material;

    this->diffuseTextureChanged(m_host->diffuseTexture());
    this->specularTextureChanged(m_host->specularTexture());
    this->bumpTextureChanged(m_host->bumpTexture());

    connect(m_host, SIGNAL(diffuseTextureChanged(QSharedPointer<Texture>)), this, SLOT(diffuseTextureChanged(QSharedPointer<Texture>)));
    connect(m_host, SIGNAL(specularTextureChanged(QSharedPointer<Texture>)), this, SLOT(specularTextureChanged(QSharedPointer<Texture>)));
    connect(m_host, SIGNAL(bumpTextureChanged(QSharedPointer<Texture>)), this, SLOT(bumpTextureChanged(QSharedPointer<Texture>)));
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

    setParent(parent);
}

Material * OpenGLMaterial::host() const {
    return m_host;
}

void OpenGLMaterial::bind() {
    if (m_openGLDiffuseTexture) m_openGLDiffuseTexture->bind();
    if (m_openGLSpecularTexture) m_openGLSpecularTexture->bind();
    if (m_openGLBumpTexture) m_openGLBumpTexture->bind();

    shaderMaterialInfo.color = m_host->color();
    shaderMaterialInfo.ambient = m_host->ambient();
    shaderMaterialInfo.diffuse = m_host->diffuse();
    shaderMaterialInfo.specular = m_host->specular();
    shaderMaterialInfo.shininess = m_host->shininess();
    shaderMaterialInfo.useDiffuseMap = !m_host->diffuseTexture().isNull();
    shaderMaterialInfo.useSpecularMap = !m_host->specularTexture().isNull();
    shaderMaterialInfo.useBumpMap = !m_host->bumpTexture().isNull();

    if (m_materialInfo == 0) {
        m_materialInfo = new OpenGLUniformBufferObject;
        m_materialInfo->create();
        m_materialInfo->bind();
        m_materialInfo->allocate(MATERIAL_INFO_BINDING_POINT, NULL, sizeof(ShaderMaterialInfo));
        m_materialInfo->release();
    }
    m_materialInfo->bind();
    m_materialInfo->write(0, &shaderMaterialInfo, sizeof(ShaderMaterialInfo));
    m_materialInfo->release();
}

void OpenGLMaterial::release() {
    if (m_openGLDiffuseTexture) m_openGLDiffuseTexture->release();
    if (m_openGLSpecularTexture) m_openGLSpecularTexture->release();
    if (m_openGLBumpTexture) m_openGLBumpTexture->release();

    shaderMaterialInfo.color = QVector3D(0, 0, 0);
    shaderMaterialInfo.useDiffuseMap = 0;
    shaderMaterialInfo.useSpecularMap = 0;
    shaderMaterialInfo.useBumpMap = 0;

    if (m_materialInfo == 0) {
        m_materialInfo = new OpenGLUniformBufferObject;
        m_materialInfo->create();
        m_materialInfo->bind();
        m_materialInfo->allocate(3, NULL, sizeof(ShaderMaterialInfo));
        m_materialInfo->release();
    }
    m_materialInfo->bind();
    m_materialInfo->write(0, &shaderMaterialInfo, sizeof(ShaderMaterialInfo));
    m_materialInfo->release();
}

void OpenGLMaterial::diffuseTextureChanged(QSharedPointer<Texture> diffuseTexture) {
    if (diffuseTexture.isNull())
        m_openGLDiffuseTexture = 0;
    else if (diffuseTexture->property("OpenGLTexturePointer").isValid())
        m_openGLDiffuseTexture = diffuseTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
    else
        m_openGLDiffuseTexture = new OpenGLTexture(diffuseTexture.data());
}

void OpenGLMaterial::specularTextureChanged(QSharedPointer<Texture> specularTexture) {
    if (specularTexture.isNull())
        m_openGLSpecularTexture = 0;
    else if (specularTexture->property("OpenGLTexturePointer").isValid())
        m_openGLSpecularTexture = specularTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
    else
        m_openGLSpecularTexture = new OpenGLTexture(specularTexture.data());
}

void OpenGLMaterial::bumpTextureChanged(QSharedPointer<Texture> bumpTexture) {
    if (bumpTexture.isNull())
        m_openGLBumpTexture = 0;
    else if (bumpTexture->property("OpenGLTexturePointer").isValid())
        m_openGLBumpTexture = bumpTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
    else
        m_openGLBumpTexture = new OpenGLTexture(bumpTexture.data());
}

void OpenGLMaterial::hostDestroyed(QObject *) {
    // Commit suicide
    delete this;
}
