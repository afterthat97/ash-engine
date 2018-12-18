#pragma once

#include <Generic/Common.h>

class Model;

class Helper {
public:
    static Model* createConeModel();
    static Model* createCubeModel();
    static Model* createCylinderModel();
    static Model* createPlaneModel();
    static Model* createSphereModel();
};