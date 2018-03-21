#include "window.h"

Window::Window(int32_t sizeX, int32_t sizeY, const char* title) {
    windowSizeX = sizeX; windowSizeY = sizeY;

    // Initialize GLFW library
    if (!glfwInit()) {
        reportError("Failed to initialize GLFW library");
        exit(-1);
    }

    // OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef APPLE_MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(sizeX, sizeY, title, NULL, NULL);
    if (window == NULL) {
        reportError("Failed to create GLFW window");
        exit(-1);
    }

    // Optimized for Retina display
    glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
    glfwMakeContextCurrent(window);
    aspectRatio = (float) frameSizeX / (float) frameSizeY;
    scaleRatio = frameSizeX / windowSizeX;
}

void Window::resize(int32_t sizeX, int32_t sizeY) {
    windowSizeX = sizeX; windowSizeY = sizeY;
    glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
    TwWindowSize(frameSizeX, frameSizeY);
    glViewport(0, 0, frameSizeX, frameSizeY);
    scaleRatio = windowSizeX ? frameSizeX / windowSizeX : 0;
}

mat4 Window::getProjMatrix() {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100000.0f);
}

void Window::screenshot() {
    uint8_t* pixels = new uint8_t[3 * frameSizeX * frameSizeY];
    string filename = "Screen Shot " + getFormatDate() + " at " + getFormatTime() + " .bmp";

    // Capture screen pixels
    glReadPixels(0, 0, frameSizeX, frameSizeY, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	
	// Call FreeImage to save screenshots
    FreeImage_Initialise(0);
    FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(pixels, frameSizeX, frameSizeY, 3 * frameSizeX, 24, 0x0000FF, 0xFF0000, 0x00FF00, 0);
    FreeImage_Save(FIF_BMP, bitmap, filename.c_str(), 0);
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

	// Clean
    delete[] pixels;
}

uint32_t Window::getFrameAreai() {
    return frameSizeX * frameSizeY;
}

float Window::getFrameAreaf() {
    return (float) getFrameAreai();
}

vec2 Window::getFrameSizefv() {
    return vec2(frameSizeX, frameSizeY);
}

vec2 Window::getWindowSizefv() {
    return vec2(windowSizeX, windowSizeY);
}

pair<uint32_t, uint32_t> Window::getFrameSizei() {
    return pair<uint32_t, uint32_t>(frameSizeX, frameSizeY);
}

pair<uint32_t, uint32_t> Window::getWindowSizei() {
    return pair<uint32_t, uint32_t>(windowSizeX, windowSizeY);
}

uint32_t Window::getScaleRatio() {
    return scaleRatio;
}

float Window::getAspectRatio() {
    return aspectRatio;
}

GLFWwindow* Window::getGLFWwindow() {
    return window;
}
