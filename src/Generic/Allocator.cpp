#include <Generic/Allocator.h>

uint32_t Allocator::meshIDCounter = 0;
uint32_t Allocator::lightIDCounter = 0;
uint32_t Allocator::textureIDCounter = 0;
uint32_t Allocator::materialIDCounter = 0;
uint32_t Allocator::modelIDCounter = 0;
uint32_t Allocator::sceneIDCounter = 0;

uint32_t Allocator::allocateMeshID() {
    return meshIDCounter++;
}

uint32_t Allocator::allocateLightID() {
    return lightIDCounter++;
}

uint32_t Allocator::allocateTextureID() {
    return textureIDCounter++;
}

uint32_t Allocator::allocateMaterialID() {
    return materialIDCounter++;
}

uint32_t Allocator::allocateModelID() {
    return modelIDCounter++;
}

uint32_t Allocator::allocateSceneID() {
    return sceneIDCounter++;
}
