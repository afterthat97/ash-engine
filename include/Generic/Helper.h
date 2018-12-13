#pragma once

#include <Generic/Common.h>

class Model;

class Helper {
public:
    static Model* createCubeModel();
    static Model* createPlaneModel();
    static Model* createSphereModel();
};