#include "axis.h"
#include "extmath.h"

Axis::Axis(btDiscreteDynamicsWorld* dynamicsWorld) {
	pos = vec3(0.0);

	vector<Vertex> verticesTransX, verticesTransY, verticesTransZ;
	vector<Vertex> verticesRotX, verticesRotY, verticesRotZ;
	vector<uint32_t> indicesTrans, indicesRot;

	Material *materialX = new Material();
	Material *materialY = new Material();
	Material* materialZ = new Material();

	materialX->diffuse = vec4(0.9, 0.2, 0.2, 1.0);
	materialY->diffuse = vec4(0.2, 0.9, 0.2, 1.0);
	materialZ->diffuse = vec4(0.2, 0.2, 0.9, 1.0);

	for (uint32_t i = 0; i < arrow_vertices.size() / 3; i++) {
		vec4 pos(arrow_vertices[i * 3 + 0], arrow_vertices[i * 3 + 1], arrow_vertices[i * 3 + 2], 1.0);
		pos.x = pos.x * 1.5;
		Vertex vertex;
		vertex.position = vec3(pos);
		verticesTransX.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(0.0, 0.0, 1.0), M_PI / 2)) * pos);
		verticesTransY.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
		verticesTransZ.push_back(vertex);
	}
	for (uint32_t i = 0; i < arrow_indices.size(); i++)
		indicesTrans.push_back(arrow_indices[i] - 1);

	for (uint32_t i = 0; i < torus_vertices.size() / 3; i++) {
		vec4 pos(torus_vertices[i * 3 + 0], torus_vertices[i * 3 + 1], torus_vertices[i * 3 + 2], 1.0);
		pos = pos * 3.0;
		Vertex vertex;
		vertex.position = vec3(pos);
		verticesRotZ.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(1.0, 0.0, 0.0), M_PI / 2)) * pos);
		verticesRotY.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
		verticesRotX.push_back(vertex);
	}
	for (uint32_t i = 0; i < torus_indices.size(); i++)
		indicesRot.push_back(torus_indices[i] - 1);

	transX = new Mesh(verticesTransX, indicesTrans, materialX, dynamicsWorld, "ATVIEW_AXIS_transX");
	transY = new Mesh(verticesTransY, indicesTrans, materialY, dynamicsWorld, "ATVIEW_AXIS_transY");
	transZ = new Mesh(verticesTransZ, indicesTrans, materialZ, dynamicsWorld, "ATVIEW_AXIS_transZ");
	rotX = new Mesh(verticesRotX, indicesRot, materialX, dynamicsWorld, "ATVIEW_AXIS_rotX");
	rotY = new Mesh(verticesRotY, indicesRot, materialY, dynamicsWorld, "ATVIEW_AXIS_rotY");
	rotZ = new Mesh(verticesRotZ, indicesRot, materialZ, dynamicsWorld, "ATVIEW_AXIS_rotZ");
}

void Axis::show() {
	transX->show();
	transY->show();
	transZ->show();
	rotX->show();
	rotY->show();
	rotZ->show();
}

void Axis::hide() {
	transX->hide();
	transY->hide();
	transZ->hide();
	rotX->hide();
	rotY->hide();
	rotZ->hide();
}

void Axis::render(Shader& shader, vec3 cameraPos) {
	glClear(GL_DEPTH_BUFFER_BIT);
	transX->setPosition(cameraPos + normalize(pos - cameraPos) * 100);
	transY->setPosition(cameraPos + normalize(pos - cameraPos) * 100);
	transZ->setPosition(cameraPos + normalize(pos - cameraPos) * 100);
	rotX->setPosition(cameraPos + normalize(pos - cameraPos) * 100);
	rotY->setPosition(cameraPos + normalize(pos - cameraPos) * 100);
	rotZ->setPosition(cameraPos + normalize(pos - cameraPos) * 100);
	transX->render(shader);
	transY->render(shader);
	transZ->render(shader);
	rotX->render(shader);
	rotY->render(shader);
	rotZ->render(shader);
}

void Axis::addTranslation(vec3 delta) {
	pos += delta;
}

void Axis::setPosition(vec3 newPos) {
	pos = newPos;
}

vec3 Axis::getPosition() {
	return pos;
}
