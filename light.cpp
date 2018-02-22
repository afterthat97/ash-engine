#include "light.h"

Light::Light(vec3 _color, btDiscreteDynamicsWorld *_dynamicsWorld): Mesh() {
	color = _color;
	dynamicsWorld = _dynamicsWorld;
	name = "ATVIEW_LIGHT";
	for (uint32_t i = 0; i < cube_vertices.size() / 3; i++) {
		vec3 pos(cube_vertices[i * 3 + 0], cube_vertices[i * 3 + 1], cube_vertices[i * 3 + 2]);
		Vertex vertex;
		vertex.position = pos * vec3(10.0);
		vertices.push_back(vertex);
	}
	minv = vec3(0.0);
	maxv = vec3(10.0);
	lenv = vec3(10.0);
	for (int i = 0; i < cube_indices.size(); i++)
		indices.push_back(cube_indices[i] - 1);

	material = new Material();
	material->diffuse = color;
	initBufferObject();
	initRigidBody();
	addToBulletDynamicsWorld();
}

Light::Light(const Light& a): Mesh(a) {
	color = a.color;
}
	
void Light::setColor(vec3 _color) {
	color = _color;
	material->diffuse = color;
}

vec3 Light::getColor() {
	return color;
}
