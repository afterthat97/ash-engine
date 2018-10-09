#ifndef MESH_H
#define MESH_H

#include "utilities.h"
#include "material.h"
#include "vertex.h"
#include "shader.h"

enum MeshType { DEFAULT, LIGHT, AXIS }; 

class Mesh {
protected:
    string name;
    vec3 pos, scale, minv, maxv, lenv;
    MeshType meshType;
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
    Mesh(void *_parent = NULL,
            MeshType _meshType = DEFAULT);
    Mesh(vector<Vertex>& _vertices,
            vector<uint32_t>& _indices,
            shared_ptr<Material> _material,
            btDiscreteDynamicsWorld* _dynamicsWorld,
            string _name = "Untitled",
            MeshType _meshType = DEFAULT);
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
	void addScaling(vec3);
    
    void setName(string);
    string getName();

    void setPosition(vec3);
    vec3 getPosition();

    void setParent(void*);
    void* getParent();
	
    void setType(MeshType);
	MeshType getType();
    
    vec3 getSize();
};

#endif
