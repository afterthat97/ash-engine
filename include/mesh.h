#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "material.h"
#include "vertex.h"
#include "shader.h"

class Mesh {
protected:
	vec3 pos;
	quat rot;
	mat4 model;
	void *parent;
	bool visible;
	int32_t selected;
	vec3 minv, maxv, lenv;
	uint32_t VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	Material* material;
	btRigidBody *meshRigidBody;
	btDiscreteDynamicsWorld *dynamicsWorld;
	void initRigidBody();
	void initBufferObject();
	void applyToBulletRigidBody();
	void addToBulletDynamicsWorld();
	void removeFromBulletDynamicsWorld();
public:
	string name;
	Mesh(void *_parent = NULL);
	Mesh(vector<Vertex>& vertices,
		 vector<uint32_t>& indices,
		 Material* material,
		 btDiscreteDynamicsWorld* dynamicsWorld,
		 string name = "Untitled Mesh");
	Mesh(const Mesh &a);
	~Mesh();
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
