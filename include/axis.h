#ifndef AXIS_H
#define AXIS_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

enum TransformMode { TRANS_X, TRANS_Y, TRANS_Z, ROT_X, ROT_Y, ROT_Z};


class Axis {
private:
    vec3 pos;
	bool visible;
	TransformMode transformMode;
    Mesh * transX, * transY, * transZ;
    Mesh * rotX, * rotY, * rotZ;
	Mesh * scaleX, * scaleY, * scaleZ;
public:
    Axis(btDiscreteDynamicsWorld*);
	~Axis();
    void show();
    void hide();
	void setTransformMode(TransformMode newMode);
	TransformMode getTransformMode();
    void render(Shader& shader, vec3 cameraPos);
    void addTranslation(vec3);
    void setPosition(vec3);
    vec3 getPosition();
};

#endif
