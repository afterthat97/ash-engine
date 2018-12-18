#include <Generic/Helper.h>
#include <Generic/Model.h>
#include <IO/Loader.h>

Model * Helper::createConeModel() {
    Model* newModel = Loader::loadFromFile(":/resources/shapes/cone.fbx");
    return newModel;
}

Model * Helper::createCubeModel() {
    Model* newModel = Loader::loadFromFile(":/resources/shapes/cube.fbx");
    return newModel;
}

Model * Helper::createCylinderModel() {
    Model* newModel = Loader::loadFromFile(":/resources/shapes/cylinder.fbx");
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
