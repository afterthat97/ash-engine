#ifndef AXIS_H
#define AXIS_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

enum TransformMode { TRANSLATION, ROTATION, SCALING };

class Axis {
private:
    vec3 pos, lastIntersection, transformAxis;
	bool visible, draging;
	TransformMode transformMode;
    Mesh * transX, * transY, * transZ;
    Mesh * rotX, * rotY, * rotZ;
	Mesh * scaleX, * scaleY, * scaleZ;
public:
    Axis(btDiscreteDynamicsWorld*);
	~Axis();
    void show();
    void hide();
	bool startDrag(Mesh* selectedMesh, vec4 raySt, vec4 rayEd);
	bool continueDrag(Mesh* selectedMesh, vec4 raySt, vec4 rayEd);
	void stopDrag();
	void setTransformMode(TransformMode newMode);
	TransformMode getTransformMode();
    void render(Shader& shader, vec3 cameraPos);
    void addTranslation(vec3);
    void setPosition(vec3);
    vec3 getPosition();
};

#endif
