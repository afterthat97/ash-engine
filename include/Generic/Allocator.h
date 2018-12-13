#pragma once

#include <Generic/Common.h>

class Allocator {
public:
    static uint32_t allocateMeshID();
    static uint32_t allocateLightID();
    static uint32_t allocateTextureID();
    static uint32_t allocateMaterialID();
    static uint32_t allocateModelID();
    static uint32_t allocateSceneID();
private:
    static uint32_t meshIDCounter;
    static uint32_t lightIDCounter;
    static uint32_t textureIDCounter;
    static uint32_t materialIDCounter;
    static uint32_t modelIDCounter;
    static uint32_t sceneIDCounter;
};