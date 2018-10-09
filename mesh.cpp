#include "mesh.h"
#include "glconfig.h"
#include "extmath.h"

Mesh::Mesh(void *_parent,
        MeshType _meshType) {
    name = "Untitled";
    meshType = _meshType;
    pos = vec3(0.0);
	scale = vec3(1.0);
    rot = quat(vec3(0.0));
    model = mat4(1.0);
    parent = _parent;
    visible = true;
    selected = false;
    minv = vec3(FLT_MAX);
    maxv = vec3(-FLT_MAX);
    lenv = vec3(0.0);
    VAO = VBO = EBO = 0;
	meshShape = NULL;
    meshRigidBody = NULL;
	meshMotionState = NULL;
    dynamicsWorld = NULL;
}

Mesh::Mesh(vector<Vertex>& _vertices,
           vector<uint32_t>& _indices,
           shared_ptr<Material> _material,
           btDiscreteDynamicsWorld *_dynamicsWorld,
           string _name,
           MeshType _meshType) {
    name = _name;
    meshType = _meshType;
    vertices = _vertices;
    indices = _indices;
    material = _material;
    dynamicsWorld = _dynamicsWorld;
    pos = vec3(0.0);
	scale = vec3(1.0);
    rot = quat(vec3(0.0));
    visible = true;
    selected = false;
    minv = vec3(FLT_MAX);
    maxv = vec3(-FLT_MAX);
    VAO = VBO = EBO = 0;
	parent = NULL;
	meshShape = NULL;
    meshRigidBody = NULL;
	meshMotionState = NULL;

    // Size of mesh
    for (uint32_t i = 0; i < vertices.size(); i++) {
        minv = minVec3(minv, vertices[i].position);
        maxv = maxVec3(maxv, vertices[i].position);
    }
    lenv = maxv - minv;

    // Reset position
    if (meshType != AXIS) {
        pos = minv;
        for (uint32_t i = 0; i < vertices.size(); i++)
            vertices[i].position -= minv;
    }

    // Model matrix
    model = translate(mat4(1.0), pos) * glm::toMat4(rot);

    // Initialize VAO, VBO, etc
    initBufferObject();

    // Initialize rigid body for bullet
    initRigidBody();

    // Add this mesh to bullet engine, preparing for mouse picking
    addToBulletDynamicsWorld();
}

Mesh::Mesh(const Mesh &a) {
    name = a.name;
    meshType = a.meshType;
    pos = a.pos;
	scale = a.scale;
    rot = a.rot;
    model = a.model;
    parent = a.parent;
    visible = true;
    selected = false;
    minv = a.minv;
    maxv = a.maxv;
    lenv = a.lenv;
    VAO = VBO = EBO = 0;
    vertices = a.vertices;
    indices = a.indices;
    material = a.material;
    meshRigidBody = NULL;
    dynamicsWorld = a.dynamicsWorld;

    // Initialize VAO, VBO, etc
    initBufferObject();

    // Initialize rigid body for bullet
    initRigidBody();

    // Add this mesh to bullet engine, preparing for mouse picking
    addToBulletDynamicsWorld();
}

Mesh::~Mesh() {
    // Clean buffer object
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

	// Clean bullet rigid body
    if (meshRigidBody) {
        dynamicsWorld->removeRigidBody(meshRigidBody);
		delete triangleMesh;
		delete meshMotionState;
		delete meshShape;
        delete meshRigidBody;
    }
    reportInfo("Mesh " + name + " has been unloaded.");
}

// Show the mesh on screen
void Mesh::show() {
	if (visible) return;
    visible = true;

    // Add it to bullet engine so it can be selected by mouse picking
    addToBulletDynamicsWorld();
}

// Hide the mesh
void Mesh::hide() {
	if (!visible) return;
    visible = false;

    // Remove it from bullet engine
    removeFromBulletDynamicsWorld();
}

// Select the mesh
void Mesh::select() {
    selected = true;
}

// Deselect the mesh
void Mesh::deselect() {
    selected = false;
}

// Render
void Mesh::render(Shader& shader) {
    if (!visible) return;
    if (VAO == 0) {
        reportError("Failed to render mesh " + name + ": Invalid VAO");
        return;
    }

    // Use and configure shader
    shader.use();
    shader.setMat4("model", model);
    shader.setInt("selected", selected);

    // Bind material
    if (material != NULL)
        material->bind(shader);

    // Render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Initialize rigid body for bullet engine
void Mesh::initRigidBody() {
	triangleMesh = new btTriangleMesh();
	for (uint32_t i = 0; i < indices.size(); i += 3) {
		vec3 v0(vertices[indices[i]].position);
		vec3 v1(vertices[indices[i + 1]].position);
		vec3 v2(vertices[indices[i + 2]].position);
		triangleMesh->addTriangle(
				btVector3(v0.x, v0.y, v0.z),
				btVector3(v1.x, v1.y, v1.z),
				btVector3(v2.x, v2.y, v2.z));
	}
	meshShape = new btBvhTriangleMeshShape(triangleMesh, false);
    meshMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w),
        btVector3(pos.x, pos.y, pos.z)));
    btRigidBody::btRigidBodyConstructionInfo meshRigidBodyCI(
        0,
        meshMotionState,
        meshShape,
        btVector3(0, 0, 0));
    meshRigidBody = new btRigidBody(meshRigidBodyCI);
    meshRigidBody->setUserPointer((void*) this);
}

// Initialize VAO, VBO, etc
void Mesh::initBufferObject() {
    // Generate object ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind vertex array object
    glBindVertexArray(VAO);

    // Bind VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    // Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));

    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tangent));

    glBindVertexArray(0);
}

// Apply transformation to bullet rigid body
void Mesh::applyToBulletRigidBody() {
    btTransform meshTransform;
    btMotionState *meshMotionState = meshRigidBody->getMotionState();
    
    // Translation
    meshTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
    
    // Rotation
    meshTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
    meshMotionState->setWorldTransform(meshTransform);

    // Scaling
	meshShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
    meshRigidBody->setWorldTransform(meshTransform);
    
    if (visible) {
        // Re-add the mesh to bullet engine to update
		removeFromBulletDynamicsWorld();
		addToBulletDynamicsWorld();
	}
}

// Add the mesh to bullet engine
void Mesh::addToBulletDynamicsWorld() {
    if (dynamicsWorld)
		dynamicsWorld->addRigidBody(meshRigidBody);
}

// Remove the mesh from bullet engine
void Mesh::removeFromBulletDynamicsWorld() {
    if (dynamicsWorld)
		dynamicsWorld->removeRigidBody(meshRigidBody);
}

// Apply translation to the mesh
void Mesh::addTranslation(vec3 delta) {
    pos += delta; minv += delta; maxv += delta;
    model = translate(mat4(1.0), pos) * glm::toMat4(rot) * glm::scale(mat4(1.0), scale);
    applyToBulletRigidBody();
}

// Apply rotation to the mesh
void Mesh::addRotation(vec3 eularAngle) {
    rot = quat(eularAngle) * rot;
    model = translate(mat4(1.0), pos) * glm::toMat4(rot) * glm::scale(mat4(1.0), scale);
    applyToBulletRigidBody();
}

// Apply scaling to the mesh
void Mesh::addScaling(vec3 scaleVector) {
	scale = scale * scaleVector;
    model = translate(mat4(1.0), pos) * glm::toMat4(rot) * glm::scale(mat4(1.0), scale);
    applyToBulletRigidBody();
}

// Set name
void Mesh::setName(string newName) {
    name = newName;
}

// Get name
string Mesh::getName() {
    return name;
}

// Set the position
void Mesh::setPosition(vec3 newPos) {
    addTranslation(newPos - pos);
}

// Get the position
vec3 Mesh::getPosition() {
    return pos;
}

// Set the parent model
void Mesh::setParent(void * _parent) {
    parent = _parent;
}

// Get the parent model
void* Mesh::getParent() {
    return parent;
}

// Set mesh type
void Mesh::setType(MeshType _meshType) {
    meshType = _meshType;
}

// Get mesh type
MeshType Mesh::getType() {
    return meshType;
}

// Get the size of the mesh
vec3 Mesh::getSize() {
    return lenv;
}

// Dump details to console
void Mesh::dumpinfo(string tab) {
    printf("%sMesh %s:\n", tab.c_str(), name.c_str());
    printf("%s  Vertices: %d\n", tab.c_str(), (int) vertices.size());
    printf("%s  Triangles: %d\n", tab.c_str(), (int) indices.size() / 3);
    if (material) material->dumpinfo(tab + "  ");
}
