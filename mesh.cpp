#include "mesh.h"
#include "glconfig.h"
#include "extmath.h"

Mesh::~Mesh() {
	// Clean
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Mesh::initBO() {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	// Tangent
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// Texture Coords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	
	glBindVertexArray(0);
}	

void Mesh::show() {
	visible = true;
}

void Mesh::hide() {
	visible = false;
}

void Mesh::select() {
	selected = true;
}

void Mesh::deselect() {
	selected = false;
}

void Mesh::render(Shader& shader) {
	if (!visible) return;

	// Use shader
	shader.use();
	shader.setMat4("model", model);
	if (name.substr(0, 11) != "ATVIEW_AXIS")
		shader.setInt("selected", selected);
	else
		shader.setInt("selected", 0);

	// Bind material
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].bind(shader);
	
	// Render object
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::addToDynamicsWorld(btDiscreteDynamicsWorld* dw) {
	dynamicsWorld = dw;
	btCollisionShape* meshShape = new btConvexHullShape(
			&vertices[0].position[0],
			vertices.size(),
			sizeof(Vertex));
	meshMotionState = new btDefaultMotionState(
			btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w),
			btVector3(pos.x, pos.y, pos.z)));
	btRigidBody::btRigidBodyConstructionInfo meshRigidBodyCI(
			0,
			meshMotionState,
			meshShape,
			btVector3(0, 0, 0));
	meshRigidBody = new btRigidBody(meshRigidBodyCI);
	meshRigidBody->setUserPointer((void*)this);
	dynamicsWorld->addRigidBody(meshRigidBody);
}

void Mesh::applyToRigidBody() {
	btTransform transform;
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
	if (meshMotionState != NULL)
		meshMotionState->setWorldTransform(transform);
	if (meshRigidBody != NULL)
		meshRigidBody->setWorldTransform(transform);
	if (dynamicsWorld != NULL) {
		dynamicsWorld->removeRigidBody(meshRigidBody);
		dynamicsWorld->addRigidBody(meshRigidBody);
	}
}

void Mesh::addTranslation(vec3 delta) {
	pos += delta; minv += delta; maxv += delta;
	model = translate(mat4(1.0), pos) * glm::toMat4(rot);
	applyToRigidBody();
}

void Mesh::setTranslation(vec3 newPos) {
	addTranslation(newPos - pos);
}

void Mesh::addRotation(vec3 eularAngle) {
	rot = quat(eularAngle) * rot;
	model = translate(mat4(1.0), pos) * glm::toMat4(rot);
	applyToRigidBody();
}

vec3 Mesh::getPosition() {
	return pos;
}

void Mesh::dumpinfo(string tab) {
	printf("%sMesh %s:\n", tab.c_str(), name.c_str());
	printf("%s  Vertices: %lu\n", tab.c_str(), vertices.size());
	printf("%s  Triangles: %lu\n", tab.c_str(), indices.size() / 3);
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].dumpinfo(tab + "  ");
}
