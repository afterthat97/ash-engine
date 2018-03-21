#include "mesh.h"
#include "glconfig.h"
#include "extmath.h"

Mesh::Mesh(void *_parent) {
    pos = vec3(0.0);
    rot = quat(vec3(0.0));
    model = mat4(1.0);
    name = "Untitled Mesh";
    parent = _parent;
    visible = true;
    selected = false;
    minv = vec3(FLT_MAX);
    maxv = vec3(-FLT_MAX);
    lenv = vec3(0.0);
    VAO = VBO = EBO = 0;
    meshRigidBody = NULL;
    dynamicsWorld = NULL;
}

Mesh::Mesh(vector<Vertex>& _vertices,
           vector<uint32_t>& _indices,
           shared_ptr<Material> _material,
           btDiscreteDynamicsWorld *_dynamicsWorld,
           string _name) {
    name = _name;
    vertices = _vertices;
    indices = _indices;
    material = _material;
    dynamicsWorld = _dynamicsWorld;
    pos = vec3(0.0);
    rot = quat(vec3(0.0));
    visible = true;
    selected = false;
    minv = vec3(FLT_MAX);
    maxv = vec3(-FLT_MAX);
    VAO = VBO = EBO = 0;
    meshRigidBody = NULL;

    // Calculate boundry
    for (uint32_t i = 0; i < vertices.size(); i++) {
        minv = minVec3(minv, vertices[i].position);
        maxv = maxVec3(maxv, vertices[i].position);
    }
    lenv = maxv - minv;

    if (name.substr(0, 11) != "ATVIEW_AXIS") {
        pos = minv;
        for (uint32_t i = 0; i < vertices.size(); i++)
            vertices[i].position -= minv;
    }
    model = translate(mat4(1.0), pos) * glm::toMat4(rot);

    initBufferObject();
    initRigidBody();
    addToBulletDynamicsWorld();
}

Mesh::Mesh(const Mesh &a) {
    pos = a.pos;
    rot = a.rot;
    model = a.model;
    name = a.name;
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

    initBufferObject();
    initRigidBody();
    addToBulletDynamicsWorld();
}

Mesh::~Mesh() {
    // Clean buffer object
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

	// Clean bullet rigid body
    if (meshRigidBody) {
        delete meshRigidBody->getMotionState();
        delete meshRigidBody->getCollisionShape();
        dynamicsWorld->removeRigidBody(meshRigidBody);
        delete meshRigidBody;
    }

    reportInfo("Mesh " + name + " has been deleted successfully.");
}

void Mesh::show() {
    visible = true;
    removeFromBulletDynamicsWorld();
}

void Mesh::hide() {
    visible = false;
    addToBulletDynamicsWorld();
}

void Mesh::select() {
    selected = true;
}

void Mesh::deselect() {
    selected = false;
}

void Mesh::render(Shader& shader) {
    if (!visible) return;
    if (VAO == 0) {
        reportError("Failed to render mesh " + name + ": Invalid VAO");
        return;
    }

    // Use shader
    shader.use();
    shader.setMat4("model", model);
    shader.setInt("selected", selected);

    // Bind material
    if (material != NULL)
        material->bind(shader);

    // Render object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::initRigidBody() {
	// Initialize bullet rigid body
    btCollisionShape* meshShape = new btConvexHullShape(
        &vertices[0].position[0],
        (int32_t) vertices.size(),
        (int32_t) sizeof(Vertex));
    btDefaultMotionState *meshMotionState = new btDefaultMotionState(
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

    // Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::applyToBulletRigidBody() {
    if (!dynamicsWorld) return;

	// Apply transform to bullet rigid body
    btTransform meshTransform;
    btMotionState *meshMotionState = meshRigidBody->getMotionState();
    meshTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
    meshTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
    meshMotionState->setWorldTransform(meshTransform);
    meshRigidBody->setWorldTransform(meshTransform);
    removeFromBulletDynamicsWorld();
    addToBulletDynamicsWorld();
}

void Mesh::addToBulletDynamicsWorld() {
    if (dynamicsWorld)
		dynamicsWorld->addRigidBody(meshRigidBody);
}

void Mesh::removeFromBulletDynamicsWorld() {
    if (dynamicsWorld)
		dynamicsWorld->removeRigidBody(meshRigidBody);
}

void Mesh::addTranslation(vec3 delta) {
    pos += delta; minv += delta; maxv += delta;
    model = translate(mat4(1.0), pos) * glm::toMat4(rot);
    applyToBulletRigidBody();
}

void Mesh::addRotation(vec3 eularAngle) {
    rot = quat(eularAngle) * rot;
    model = translate(mat4(1.0), pos) * glm::toMat4(rot);
    applyToBulletRigidBody();
}

void Mesh::setPosition(vec3 newPos) {
    addTranslation(newPos - pos);
}

vec3 Mesh::getPosition() {
    return pos;
}

void Mesh::setParent(void * _parent) {
    parent = _parent;
}

void* Mesh::getParent() {
    return parent;
}

vec3 Mesh::getSize() {
    return lenv;
}

void Mesh::dumpinfo(string tab) {
    printf("%sMesh %s:\n", tab.c_str(), name.c_str());
    printf("%s  Vertices: %d\n", tab.c_str(), (int) vertices.size());
    printf("%s  Triangles: %d\n", tab.c_str(), (int) indices.size() / 3);
    if (material) material->dumpinfo(tab + "  ");
}
