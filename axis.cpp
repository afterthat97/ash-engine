#include "axis.h"
#include "extmath.h"

Axis::Axis(btDiscreteDynamicsWorld* dynamicsWorld) {
    pos = vec3(0.0);

    vector<Vertex> verticesTransX, verticesTransY, verticesTransZ;
    vector<Vertex> verticesRotX, verticesRotY, verticesRotZ;
	vector<Vertex> verticesScaleX, verticesScaleY, verticesScaleZ;

    vector<uint32_t> indicesTrans, indicesRot, indicesScale;

    // Materials for axis X, Y, and Z
    shared_ptr<Material> materialX(new Material());
    shared_ptr<Material> materialY(new Material());
    shared_ptr<Material> materialZ(new Material());

    materialX->diffuse = vec4(0.9, 0.2, 0.2, 1.0);
    materialY->diffuse = vec4(0.2, 0.9, 0.2, 1.0);
    materialZ->diffuse = vec4(0.2, 0.2, 0.9, 1.0);

    // Generate vertex and index array for translation axes
    for (uint32_t i = 0; i < transAxisVertices.size() / 3; i++) {
        vec4 pos(transAxisVertices[i * 3 + 0], transAxisVertices[i * 3 + 1], transAxisVertices[i * 3 + 2], 1.0);
        Vertex vertex;
        vertex.position = vec3(pos);
        verticesTransX.push_back(vertex);
        vertex.position = vec3(mat4(rotate(vec3(0.0, 0.0, 1.0), M_PI / 2)) * pos);
        verticesTransY.push_back(vertex);
        vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
        verticesTransZ.push_back(vertex);
    }
    for (uint32_t i = 0; i < transAxisIndices.size(); i++)
        indicesTrans.push_back(transAxisIndices[i] - 1);

    // Generate vertex and index array for rotation axes
    for (uint32_t i = 0; i < rotAxisVertices.size() / 3; i++) {
        vec4 pos(rotAxisVertices[i * 3 + 0], rotAxisVertices[i * 3 + 1], rotAxisVertices[i * 3 + 2], 1.0);
        Vertex vertex;
        vertex.position = vec3(pos);
        verticesRotZ.push_back(vertex);
        vertex.position = vec3(mat4(rotate(vec3(1.0, 0.0, 0.0), M_PI / 2)) * pos);
        verticesRotY.push_back(vertex);
        vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
        verticesRotX.push_back(vertex);
    }
    for (uint32_t i = 0; i < rotAxisIndices.size(); i++)
        indicesRot.push_back(rotAxisIndices[i] - 1);

    // Generate vertex and index array for scale axes
    for (uint32_t i = 0; i < scaleAxisVertices.size() / 3; i++) {
        vec4 pos(scaleAxisVertices[i * 3 + 0], scaleAxisVertices[i * 3 + 1], scaleAxisVertices[i * 3 + 2], 1.0);
        Vertex vertex;
        vertex.position = vec3(pos);
        verticesScaleX.push_back(vertex);
        vertex.position = vec3(mat4(rotate(vec3(0.0, 0.0, 1.0), M_PI / 2)) * pos);
        verticesScaleY.push_back(vertex);
        vertex.position = vec3(mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * pos);
        verticesScaleZ.push_back(vertex);
    }
    for (uint32_t i = 0; i < scaleAxisIndices.size(); i++)
        indicesScale.push_back(scaleAxisIndices[i] - 1);

    // Construct translation axes and rotation axes
    transX = new Mesh(verticesTransX, indicesTrans, materialX, dynamicsWorld, "ATVIEW_AXIS_transX");
    transY = new Mesh(verticesTransY, indicesTrans, materialY, dynamicsWorld, "ATVIEW_AXIS_transY");
    transZ = new Mesh(verticesTransZ, indicesTrans, materialZ, dynamicsWorld, "ATVIEW_AXIS_transZ");
    rotX = new Mesh(verticesRotX, indicesRot, materialX, dynamicsWorld, "ATVIEW_AXIS_rotX");
    rotY = new Mesh(verticesRotY, indicesRot, materialY, dynamicsWorld, "ATVIEW_AXIS_rotY");
    rotZ = new Mesh(verticesRotZ, indicesRot, materialZ, dynamicsWorld, "ATVIEW_AXIS_rotZ");
    scaleX = new Mesh(verticesScaleX, indicesScale, materialX, dynamicsWorld, "ATVIEW_AXIS_scaleX");
    scaleY = new Mesh(verticesScaleY, indicesScale, materialY, dynamicsWorld, "ATVIEW_AXIS_scaleY");
    scaleZ = new Mesh(verticesScaleZ, indicesScale, materialZ, dynamicsWorld, "ATVIEW_AXIS_scaleZ");
}

Axis::~Axis() {
	delete transX;
	delete transY;
	delete transZ;
	delete rotX;
	delete rotY;
	delete rotZ;
	delete scaleX;
	delete scaleY;
	delete scaleZ;
}

void Axis::show() {
	visible = true;
}

void Axis::hide() {
	visible = false;
}

void Axis::render(Shader& shader, vec3 cameraPos) {
	if (visible == false) return;
    glClear(GL_DEPTH_BUFFER_BIT);

	//if (transformMode == TRANS_X || transformMode == TRANS_Y || transformMode == TRANS_Z) {
		transX->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		transY->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		transZ->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		transX->render(shader);
		transY->render(shader);
		transZ->render(shader);
	//}
	
	//if (transformMode == ROT_X || transformMode == ROT_Y || transformMode == ROT_Z) {
		rotX->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		rotY->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		rotZ->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		rotX->render(shader);
		rotY->render(shader);
		rotZ->render(shader);
	//}

	//if (transformMode == SCALE_X || transformMode == SCALE_Y || transformMode == SCALE_Z) {
		scaleX->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		scaleY->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
		scaleZ->setPosition(cameraPos + normalize(pos - cameraPos) * 200);
	    scaleX->render(shader);
		scaleY->render(shader);
		scaleZ->render(shader);
	//}
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
