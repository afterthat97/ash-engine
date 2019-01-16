#include <OpenGL/OpenGLConfig.h>

QVector3D OpenGLConfig::backgroundColor = QVector3D(0.7f, 0.7f, 0.7f);
bool OpenGLConfig::enableLighting = true;
bool OpenGLConfig::enableGridline = true;
bool OpenGLConfig::enableWireFrame = false;

QVector<ShaderAttributeConfig> OpenGLConfig::getShaderAttributeConfig() {
    QVector<ShaderAttributeConfig> config;
    config.push_back(ShaderAttributeConfig("position", 0, 3, sizeof(Vertex), (void*) offsetof(Vertex, position)));
    config.push_back(ShaderAttributeConfig("normal", 1, 3, sizeof(Vertex), (void*) offsetof(Vertex, normal)));
    config.push_back(ShaderAttributeConfig("tangent", 2, 3, sizeof(Vertex), (void*) offsetof(Vertex, tangent)));
    config.push_back(ShaderAttributeConfig("bitangent", 3, 3, sizeof(Vertex), (void*) offsetof(Vertex, bitangent)));
    config.push_back(ShaderAttributeConfig("texCoords", 4, 2, sizeof(Vertex), (void*) offsetof(Vertex, texCoords)));
    return config;
}

void OpenGLConfig::setBackgroundColor(QVector3D color) {
    backgroundColor = color;
}

QVector3D OpenGLConfig::getBackgroundColor() {
    return backgroundColor;
}

void OpenGLConfig::setEnableLighting(bool enabled) {
    enableLighting = enabled;
}

bool OpenGLConfig::isLightingEnabled() {
    return enableLighting;
}

void OpenGLConfig::setEnableGridline(bool enabled) {
    enableGridline = enabled;
}

bool OpenGLConfig::isGridlineEnabled() {
    return enableGridline;
}

void OpenGLConfig::setEnableWireFrame(bool enabled) {
    enableWireFrame = enabled;
}

bool OpenGLConfig::isWireFrameEnabled() {
    return enableWireFrame;
}