#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "material.h"
#include "vertex.h"
#include "shader.h"

class Mesh {
protected:
	bool visible;
	int32_t selected;
	uint32_t VAO, VBO, EBO;
	quat rot;
	mat4 model;
	btRigidBody* meshRigidBody;
	btDefaultMotionState* meshMotionState;
	btDiscreteDynamicsWorld* dynamicsWorld;
public:
	vec3 pos;
	string name;
	vec3 minv, maxv, lenv;
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Material> materials;
	Mesh() {
		VAO = VBO = EBO = 0;
		minv = vec3(FLT_MAX);
		maxv = vec3(-FLT_MAX);
		lenv = vec3(0.0);
		pos = vec3(0.0);
		rot = quat(vec3(0.0));
		model = mat4(1.0);
		visible = true;
		meshRigidBody = NULL;
		meshMotionState = NULL;
		dynamicsWorld = NULL;
	}
	~Mesh();
	void initBO();
	void show();
	void hide();
	void select();
	void deselect();
	void render(Shader&);
	void dumpinfo(string);
	void addToDynamicsWorld(btDiscreteDynamicsWorld*);
	void applyToRigidBody();
	void addTranslation(vec3);
	void setTranslation(vec3);
	void addRotation(vec3);
	vec3 getPosition();
};

#endif
