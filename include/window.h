#ifndef WINDOW_H
#define WINDOW_H

#include "utilities.h"

class Window {
private:
    int32_t windowSizeX, windowSizeY;
    int32_t frameSizeX, frameSizeY;
    int32_t scaleRatio;
    float aspectRatio;
    GLFWwindow* window;
public:
    Window() {};
    Window(int32_t sizeX, int32_t sizeY, const char* title);
    void resize(int32_t sizeX, int32_t sizeY);
    mat4 getProjMatrix();
    void screenshot();
    uint32_t getFrameAreai();
    float getFrameAreaf();
    vec2 getFrameSizefv();
    vec2 getWindowSizefv();
    pair<uint32_t, uint32_t> getFrameSizei();
    pair<uint32_t, uint32_t> getWindowSizei();
    uint32_t getScaleRatio();
    float getAspectRatio();
    GLFWwindow* getGLFWwindow();
};

#endif
