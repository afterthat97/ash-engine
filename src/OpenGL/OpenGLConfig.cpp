#include <OpenGL/OpenGLConfig.h>

QVector3D OpenGLConfig::backgroundColor = QVector3D(0.7f, 0.7f, 0.7f);
bool OpenGLConfig::enableWireFrame = false;

void OpenGLConfig::setBackgroundColor(QVector3D color) {
    backgroundColor = color;
}

QVector3D OpenGLConfig::getBackgroundColor() {
    return backgroundColor;
}

void OpenGLConfig::setEnableWireFrame(bool enabled) {
    enableWireFrame = enabled;
}

bool OpenGLConfig::isWireFrameEnabled() {
    return enableWireFrame;
}