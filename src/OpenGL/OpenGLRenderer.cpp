#include <OpenGL/OpenGLRenderer.h>
#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLConfig.h>
#include <Generic/Gridline.h>
#include <UI/Common.h>

OpenGLRenderer::OpenGLRenderer(QWidget* _targetWidget) {
    shader = NULL;
    targetWidget = _targetWidget;
}

OpenGLRenderer::~OpenGLRenderer() {
    if (shader) delete shader;
}

// Load, compile and link shaders
void OpenGLRenderer::loadShader(QString vertexShaderFile, QString fragmentShaderFile) {
    shader = new QOpenGLShaderProgram;
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile))
        QMessageBox::critical(0, "Error compiling vertex shader", shader->log());
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile))
        QMessageBox::critical(0, "Error compiling fragment shader", shader->log());
    if (!shader->link())
        QMessageBox::critical(0, "Error linking shaders", shader->log());
    vector<ShaderAttributeConfig> config = OpenGLConfig::getShaderAttributeConfig();
    for (uint32_t i = 0; i < config.size(); i++)
        shader->bindAttributeLocation(config[i].name.c_str(), config[i].indx);
}

// Render a generic scene
void OpenGLRenderer::render(Scene* scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(OpenGLConfig::getBackgroundColor()[0], OpenGLConfig::getBackgroundColor()[1], OpenGLConfig::getBackgroundColor()[2], 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, OpenGLConfig::isWireFrameEnabled() ? GL_LINE : GL_FILL);

    QMatrix4x4 projMat;
    projMat.perspective(45.0f, float(targetWidget->width()) / targetWidget->height(), 0.1f, 100000.0f);
    QMatrix4x4 viewMat = scene->getCamera()->getViewMatrix();

    shader->bind();
    shader->setUniformValue("projMat", projMat);
    shader->setUniformValue("viewMat", viewMat);
    shader->setUniformValue("viewPos", scene->getCamera()->getPosition());

    if (OpenGLConfig::isGridlineEnabled()) {
        // "enableLighting" must be set to false when rendering gridline
        shader->setUniformValue("enableLighting", false);
        renderMesh(Gridline::getGridlineMesh());
    }

    if (OpenGLConfig::isLightingEnabled()) {
        // "enableLighting" must be set to false when rendering lights
        shader->setUniformValue("enableLighting", false);
        shader->setUniformValue("lightNum", (GLint) scene->getLights().size());
        for (uint32_t i = 0; i < scene->getLights().size(); i++)
            renderLight(scene->getLights()[i], i);
    }
    
    // Set "enableLighting" according to the configuration
    shader->setUniformValue("enableLighting", OpenGLConfig::isLightingEnabled());
    for (uint32_t i = 0; i < scene->getModels().size(); i++)
        renderModel(scene->getModels()[i]);
}

// Render a generic model
void OpenGLRenderer::renderModel(Model * model) {
    for (uint32_t i = 0; i < model->getChildren().size(); i++)
        renderModel(model->getChildren()[i]);
    for (uint32_t i = 0; i < model->getMeshes().size(); i++)
        renderMesh(model->getMeshes()[i]);
}

// Render a generic mesh
void OpenGLRenderer::renderMesh(Mesh * mesh) {
    renderMaterial(mesh->getMaterial());

    // Convert generic mesh to specific OpenGLMesh
    OpenGLMesh* openGLMesh = OpenGLManager::getOpenGLMesh(mesh);
    shader->setUniformValue("modelMat", openGLMesh->getModelMatrix());

    // Bind OpenGLMesh(VAO, VBO...) and render it
    openGLMesh->bind();
    openGLMesh->render();
    openGLMesh->release();
}

// Render a generic material
void OpenGLRenderer::renderMaterial(Material * material) {
    shader->setUniformValue("material.hasDiffuseMap", false);
    shader->setUniformValue("material.hasSpecularMap", false);
    shader->setUniformValue("material.hasNormalMap", false);
    shader->setUniformValue("material.ambientRGB", material->getAmbientColor());
    shader->setUniformValue("material.diffuseRGB", material->getDiffuseColor());
    shader->setUniformValue("material.specularRGB", material->getSpecularColor());
    shader->setUniformValue("material.shininess", material->getShininess());
    for (uint32_t i = 0; i < material->getTextures().size(); i++)
        renderTexture(material->getTextures()[i]);
}

// Render a generic texture
void OpenGLRenderer::renderTexture(Texture * texture) {
    // Convert generic texture to specific OpenGLTexture
    OpenGLTexture* openGLTexture = OpenGLManager::getOpenGLTexture(texture);
    if (texture->getType() == Texture::Diffuse && texture->isEnabled()) { // Diffuse map
        openGLTexture->bind(0);
        shader->setUniformValue("material.diffuseMap", 0);
        shader->setUniformValue("material.hasDiffuseMap", true);
    } else if (texture->getType() == Texture::Specular && texture->isEnabled()) { // Specular map
        openGLTexture->bind(1);
        shader->setUniformValue("material.specularMap", 1);
        shader->setUniformValue("material.hasSpecularMap", true);
    } else if (texture->getType() == Texture::Normal && texture->isEnabled()) { // Normal map
        openGLTexture->bind(2);
        shader->setUniformValue("material.normalMap", 2);
        shader->setUniformValue("material.hasNormalMap", true);
    }
}

void OpenGLRenderer::renderLight(Light * light, uint32_t indx) {
    // Set light properties
    shader->setUniformValue(("lights[" + to_string(indx) + "].pos").c_str(), light->getPosition());
    shader->setUniformValue(("lights[" + to_string(indx) + "].color").c_str(), light->getColor());
    shader->setUniformValue(("lights[" + to_string(indx) + "].enableAttenuation").c_str(), light->isAttenuationEnabled());
    shader->setUniformValue(("lights[" + to_string(indx) + "].attenuationQuadratic").c_str(), light->getAttenuationQuadratic());
    shader->setUniformValue(("lights[" + to_string(indx) + "].attenuationLinear").c_str(), light->getAttenuationLinear());
    shader->setUniformValue(("lights[" + to_string(indx) + "].attenuationConstant").c_str(), light->getAttenuationConstant());

    // Render the mesh shape of light (a light bulb)
    renderModel(light->getLightBulbModel());
}
