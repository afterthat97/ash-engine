#pragma once

#include <Common.h>

class OpenGLConfig {
public:
    static void setBackgroundColor(QVector3D color);
    static QVector3D getBackgroundColor();

    static void setEnableWireFrame(bool enabled);
    static bool isWireFrameEnabled();

private:
    static QVector3D backgroundColor;
    static bool enableWireFrame;
};
