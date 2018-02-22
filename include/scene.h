#ifndef SCENE_H
#define SCENE_H

#include "utilities.h"
#include "shader.h"
#include "model.h"
#include "material.h"

class Scene {
private:
	vec3 pos;
	quat rot;
	mat4 model;
	vector<Model*> models;
	vector<Material*> materials;
public:
	string name;
	Scene(string, btDiscreteDynamicsWorld*);
	void addModel(Model*);
	void addMaterial(Material*);
	void show();
	void hide();
	void select();
	void deselect();
	void render(Shader&);
	void dumpinfo(string);
	void addTranslation(vec3);
	void addRotation(vec3);
	void setPosition(vec3);
	vec3 getPosition();
};

#endif
