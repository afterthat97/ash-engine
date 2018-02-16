#include "axis.h"
#include "extmath.h"

vector<float> arrow_vertices {
	9.500000f, -0.951057f, -0.309017f,
	9.500000f, -0.809018f, -0.587786f,
	9.500000f, -0.587786f, -0.809017f,
	9.500000f, -0.309017f, -0.951057f,
	9.500000f, -0.000000f, -1.000000f,
	9.500000f, 0.309017f, -0.951057f,
	9.500000f, 0.587785f, -0.809017f,
	9.500000f, 0.809017f, -0.587785f,
	9.500000f, 0.951057f, -0.309017f,
	9.500000f, 1.000000f, 0.000000f,
	9.500000f, 0.951057f, 0.309017f,
	9.500000f, 0.809017f, 0.587785f,
	9.500000f, 0.587785f, 0.809017f,
	9.500000f, 0.309017f, 0.951057f,
	9.500000f, 0.000000f, 1.000000f,
	9.500000f, -0.309017f, 0.951057f,
	9.500000f, -0.587785f, 0.809017f,
	9.500000f, -0.809017f, 0.587785f,
	9.500000f, -0.951057f, 0.309017f,
	9.500000f, -1.000000f, 0.000000f,
	12.500000f, 0.000000f, 0.000000f,
	0.000000f, -0.250000f, 0.250000f,
	10.000000f, -0.250000f, 0.250000f,
	0.000000f, 0.250000f, 0.250000f,
	10.000000f, 0.250000f, 0.250000f,
	0.000000f, 0.250000f, -0.250000f,
	10.000000f, 0.250000f, -0.250000f,
	0.000000f, -0.250000f, -0.250000f,
	10.000000f, -0.250000f, -0.250000f
};

vector<uint32_t> arrow_indices {
	1, 20, 2,
	20, 19, 2,
	19, 18, 2,
	18, 17, 2,
	17, 16, 2,
	16, 15, 2,
	15, 14, 2,
	14, 13, 2,
	13, 12, 2,
	12, 11, 2,
	11, 10, 2,
	10, 9, 2,
	9, 8, 2,
	8, 7, 2,
	7, 6, 2,
	6, 5, 2,
	5, 4, 2,
	4, 3, 2,
	1, 2, 21,
	2, 3, 21,
	3, 4, 21,
	4, 5, 21,
	5, 6, 21,
	6, 7, 21,
	7, 8, 21,
	8, 9, 21,
	9, 10, 21,
	10, 11, 21,
	11, 12, 21,
	12, 13, 21,
	13, 14, 21,
	14, 15, 21,
	15, 16, 21,
	16, 17, 21,
	17, 18, 21,
	18, 19, 21,
	19, 20, 21,
	20, 1, 21,
	22, 23, 24,
	24, 23, 25,
	24, 25, 26,
	26, 25, 27,
	26, 27, 28,
	28, 27, 29,
	28, 29, 22,
	22, 29, 23,
	23, 29, 25,
	25, 29, 27,
	28, 22, 26,
	26, 22, 24
};

vector<float> torus_vertices {
	0.000000f, 2.508359f, 0.000150f,
	0.386087f, 2.477580f, 0.000193f,
	0.767541f, 2.386000f, 0.000365f,
	1.129976f, 2.235874f, -0.000365f,
	1.464463f, 2.030901f, -0.000247f,
	1.762767f, 1.776128f, -0.000129f,
	2.017543f, 1.477824f, -0.000193f,
	2.222518f, 1.143337f, 0.000193f,
	2.372644f, 0.780903f, 0.000139f,
	2.464222f, 0.399447f, 0.000145f,
	2.495001f, 0.000000f, 0.000135f,
	0.000000f, 3.258357f, -0.000075f,
	0.503411f, 3.218344f, 0.000526f,
	0.999303f, 3.099291f, -0.000526f,
	1.470468f, 2.904128f, 0.000418f,
	1.905302f, 2.637663f, 0.000354f,
	2.293099f, 2.306456f, -0.000354f,
	2.624307f, 1.918662f, -0.000274f,
	2.890774f, 1.483830f, 0.000274f,
	3.085936f, 1.012666f, 0.000274f,
	3.204989f, 0.516772f, 0.000215f,
	3.245003f, 0.000000f, -0.000064f
};

vector<uint32_t> torus_indices {
	12, 1, 13,
	13, 1, 2,
	13, 2, 14,
	14, 2, 3,
	14, 3, 15,
	15, 3, 4,
	15, 4, 16,
	16, 4, 5,
	16, 5, 17,
	17, 5, 6,
	17, 6, 18,
	18, 6, 7,
	18, 7, 19,
	19, 7, 8,
	19, 8, 20,
	20, 8, 9,
	20, 9, 21,
	21, 9, 10,
	21, 10, 22,
	22, 10, 11
};

Axis::Axis() {
	transX.name = "ATVIEW_AXIS_transX";
	transY.name = "ATVIEW_AXIS_transY";
	transZ.name = "ATVIEW_AXIS_transZ";
	rotX.name = "ATVIEW_AXIS_rotX";
	rotY.name = "ATVIEW_AXIS_rotY";
	rotZ.name = "ATVIEW_AXIS_rotZ";
	for (uint32_t i = 0; i < arrow_vertices.size() / 3; i++) {
		vec4 pos(arrow_vertices[i * 3 + 0], arrow_vertices[i * 3 + 1], arrow_vertices[i * 3 + 2], 1.0);
		pos.x = pos.x * 1.5;
		Vertex vertex;
		vertex.position = vec3(pos);
		transX.vertices.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(0.0, 0.0, 1.0), M_PI / 2)) * pos);
		transY.vertices.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
		transZ.vertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < torus_vertices.size() / 3; i++) {
		vec4 pos(torus_vertices[i * 3 + 0], torus_vertices[i * 3 + 1], torus_vertices[i * 3 + 2], 1.0);
		pos = pos * 3.0;
		Vertex vertex;
		vertex.position = vec3(pos);
		rotZ.vertices.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(1.0, 0.0, 0.0), M_PI / 2)) * pos);
		rotY.vertices.push_back(vertex);
		vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
		rotX.vertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < arrow_indices.size(); i++) {
		transX.indices.push_back(arrow_indices[i] - 1);
		transY.indices.push_back(arrow_indices[i] - 1);
		transZ.indices.push_back(arrow_indices[i] - 1);
	}
	for (uint32_t i = 0; i < torus_indices.size(); i++) {
		rotX.indices.push_back(torus_indices[i] - 1);
		rotY.indices.push_back(torus_indices[i] - 1);
		rotZ.indices.push_back(torus_indices[i] - 1);
	}
	Material material;
	material.diffuse = vec4(0.9, 0.2, 0.2, 1.0);
	transX.materials.push_back(material);
	rotX.materials.push_back(material);
	material.diffuse = vec4(0.2, 0.9, 0.2, 1.0);
	transY.materials.push_back(material);
	rotY.materials.push_back(material);
	material.diffuse = vec4(0.2, 0.2, 0.9, 1.0);
	transZ.materials.push_back(material);
	rotZ.materials.push_back(material);
}

void Axis::addToDynamicsWorld(btDiscreteDynamicsWorld* dw) {
	transX.addToDynamicsWorld(dw);
	transY.addToDynamicsWorld(dw);
	transZ.addToDynamicsWorld(dw);
	rotX.addToDynamicsWorld(dw);
	rotY.addToDynamicsWorld(dw);
	rotZ.addToDynamicsWorld(dw);
}

void Axis::applyToRigidBody() {
	transX.applyToRigidBody();
	transY.applyToRigidBody();
	transZ.applyToRigidBody();
	rotX.applyToRigidBody();
	rotY.applyToRigidBody();
	rotZ.applyToRigidBody();
}

void Axis::initBO() {
	transX.initBO();
	transY.initBO();
	transZ.initBO();
	rotX.initBO();
	rotY.initBO();
	rotZ.initBO();
}

void Axis::show() {
	transX.show();
	transY.show();
	transZ.show();
	rotX.show();
	rotY.show();
	rotZ.show();
}

void Axis::hide() {
	transX.hide();
	transY.hide();
	transZ.hide();
	rotX.hide();
	rotY.hide();
	rotZ.hide();
}

void Axis::render(Shader& shader, vec3 cameraPos) {
	glClear(GL_DEPTH_BUFFER_BIT);
	transX.setTranslation(cameraPos + normalize(pos - cameraPos) * 100);
	transY.setTranslation(cameraPos + normalize(pos - cameraPos) * 100);
	transZ.setTranslation(cameraPos + normalize(pos - cameraPos) * 100);
	rotX.setTranslation(cameraPos + normalize(pos - cameraPos) * 100);
	rotY.setTranslation(cameraPos + normalize(pos - cameraPos) * 100);
	rotZ.setTranslation(cameraPos + normalize(pos - cameraPos) * 100);
	transX.render(shader);
	transY.render(shader);
	transZ.render(shader);
	rotX.render(shader);
	rotY.render(shader);
	rotZ.render(shader);
}

void Axis::addTranslation(vec3 delta) {
	pos += delta;
}

void Axis::setTranslation(vec3 newPos) {
	pos = newPos;
}

vec3 Axis::getPosition() {
	return pos;
}
