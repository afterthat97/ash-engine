#ifndef AXIS_H
#define AXIS_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

class Axis {
private:
    vec3 pos;
    Mesh *transX, *transY, *transZ;
    Mesh *rotX, *rotY, *rotZ;
public:
    Axis(btDiscreteDynamicsWorld*);
    void show();
    void hide();
    void render(Shader& shader, vec3 cameraPos);
    void addTranslation(vec3);
    void setPosition(vec3);
    vec3 getPosition();
};

#endif
