#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "material.h"
#include "vertex.h"
#include "shader.h"

class Mesh {
protected:
    vec3 pos, scale, minv, maxv, lenv;
    quat rot;
    mat4 model;
    void *parent;
    bool visible;
    int32_t selected, id;
    uint32_t VAO, VBO, EBO;
    vector<Vertex> vertices;
    vector<uint32_t> indices;
    shared_ptr<Material> material;
	btTriangleMesh* triangleMesh;
    btCollisionShape* meshShape;
    btRigidBody *meshRigidBody;
    btDefaultMotionState *meshMotionState;
    btDiscreteDynamicsWorld *dynamicsWorld;
    void initRigidBody();
    void initBufferObject();
    void applyToBulletRigidBody();
    void addToBulletDynamicsWorld();
    void removeFromBulletDynamicsWorld();
public:
    // Name is important as the program tells the difference
    // among meshes, lights and axes according to their names.
    string name;
    Mesh(void *_parent = NULL);
    Mesh(vector<Vertex>& vertices,
         vector<uint32_t>& indices,
         shared_ptr<Material> material,
         btDiscreteDynamicsWorld* dynamicsWorld,
         string name = "Untitled Mesh",
		 int32_t newid = -1);
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
	void addScale(vec3);
    void setPosition(vec3);
    vec3 getPosition();
    void setParent(void*);
    void* getParent();
	void setID(int32_t);
	int32_t getID();
    vec3 getSize();
};

#endif
