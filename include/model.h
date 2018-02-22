#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "utilities.h"

class Model {
private:
	vec3 pos;
	void* parent;
	vector<Mesh*> meshes;
	vector<Model*> children;
public:
	string name;
	Model(void *_parent = NULL);
	~Model();
	void addMesh(Mesh*);
	void addChildren(Model*);
	void removeMesh(Mesh*);
	void removeChildren(Model*);
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
	void setParent(void*);
	void* getParent();
};

#endif
