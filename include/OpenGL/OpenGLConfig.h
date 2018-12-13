#pragma once

#include <OpenGL/Common.h>
#include <Generic/Vertex.h>
#include <Generic/Texture.h>

struct ShaderAttributeConfig {
    string name;
    GLuint indx, size;
    GLsizei stride;
    const void* offset;
    ShaderAttributeConfig(string _name, GLuint _indx, GLuint _size, GLsizei _stride, const void * _offset) {
        name = _name;
        indx = _indx;
        size = _size;
        stride = _stride;
        offset = _offset;
    }
};

class OpenGLConfig {
public:
    static vector<ShaderAttributeConfig> getShaderAttributeConfig();

    static void setBackgroundColor(QVector3D color);
    static QVector3D getBackgroundColor();

    static void setEnableLighting(bool enabled);
    static bool isLightingEnabled();

    static void setEnableGridline(bool enabled);
    static bool isGridlineEnabled();

    static void setEnableWireFrame(bool enabled);
    static bool isWireFrameEnabled();

private:
    static QVector3D backgroundColor;
    static bool enableLighting;
    static bool enableGridline;
    static bool enableWireFrame;
};