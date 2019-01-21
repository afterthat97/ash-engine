#include <OpenGL/OpenGLMaterial.h>
#include <OpenGL/OpenGLTexture.h>
#include <OpenGL/OpenGLManager.h>

OpenGLMaterial::OpenGLMaterial(Material * material) {
    m_host = material;
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

Material * OpenGLMaterial::host() const {
    return m_host;
}

void OpenGLMaterial::bind(QOpenGLShaderProgram * shader) {
    shader->setUniformValue("material.ambient", m_host->ambient());
    shader->setUniformValue("material.diffuse", m_host->diffuse());
    shader->setUniformValue("material.shininess", m_host->shininess());
    shader->setUniformValue("material.diffuseMap", 0);
    shader->setUniformValue("material.specularMap", 1);
    shader->setUniformValue("material.bumpMap", 2);

    if (m_host->diffuseTexture().isNull()) {
        shader->setUniformValue("material.color", m_host->color());
    } else {
        shader->setUniformValue("material.color", QVector3D(0, 0, 0));
        OpenGLManager<Texture, OpenGLTexture>::currentManager()->getOpenGLObject(m_host->diffuseTexture().data())->bind(shader);
    }
    if (m_host->specularTexture().isNull()) {
        shader->setUniformValue("material.specular", m_host->specular());
    } else {
        shader->setUniformValue("material.specular", 0.0f);
        OpenGLManager<Texture, OpenGLTexture>::currentManager()->getOpenGLObject(m_host->specularTexture().data())->bind(shader);
    }
    if (m_host->bumpTexture().isNull()) {
        shader->setUniformValue("material.bump", QVector3D(0.5, 0.5, 1));
    } else {
        shader->setUniformValue("material.bump", QVector3D(0, 0, 0));
        OpenGLManager<Texture, OpenGLTexture>::currentManager()->getOpenGLObject(m_host->bumpTexture().data())->bind(shader);
    }
}

void OpenGLMaterial::release(QOpenGLShaderProgram * shader) {
    shader->setUniformValue("material.color", QVector3D(0.0f, 0.0f, 0.0f));
    if (!m_host->diffuseTexture().isNull())
        OpenGLManager<Texture, OpenGLTexture>::currentManager()->getOpenGLObject(m_host->diffuseTexture().data())->release();
    if (!m_host->specularTexture().isNull())
        OpenGLManager<Texture, OpenGLTexture>::currentManager()->getOpenGLObject(m_host->specularTexture().data())->release();
    if (!m_host->bumpTexture().isNull())
        OpenGLManager<Texture, OpenGLTexture>::currentManager()->getOpenGLObject(m_host->bumpTexture().data())->release();
}

void OpenGLMaterial::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<Material, OpenGLMaterial>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
