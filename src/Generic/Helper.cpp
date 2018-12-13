#include <Generic/Helper.h>
#include <Generic/Model.h>
#include <IO/Loader.h>

Model * Helper::createCubeModel() {
    Model* newModel = Loader::loadFromFile(":/resources/shapes/cube.fbx");
    return newModel;
}

Model * Helper::createPlaneModel() {
    Model* newModel = Loader::loadFromFile(":/resources/shapes/plane.fbx");
    return newModel;
}

Model * Helper::createSphereModel() {
    Model* newModel = Loader::loadFromFile(":/resources/shapes/sphere.fbx");
    return newModel;
}
