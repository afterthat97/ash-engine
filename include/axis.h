#ifndef AXIS_H
#define AXIS_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

class Axis {
private:
	Mesh transX, transY, transZ;
	Mesh rotX, rotY, rotZ;
	Mesh scaleX, scaleY, scaleZ;
	vec3 pos;
public:
	Axis();
	void addToDynamicsWorld(btDiscreteDynamicsWorld*);
	void applyToRigidBody();
	void initBO();
	void show();
	void hide();
	void render(Shader&, vec3);
	void addTranslation(vec3);
	void setTranslation(vec3);
	vec3 getPosition();
};

#endif
