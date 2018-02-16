#include "light.h"

// Looks like a cube

const float light_size = 10.0f;

vector<float> light_vertices {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
};

vector<float> light_indices {
	1, 7, 5,
	1, 3, 7,
	1, 4, 3,
	1, 2, 4,
	3, 8, 7,
	3, 4, 8,
	5, 7, 8,
	5, 8, 6,
	1, 5, 6,
	1, 6, 2,
	2, 6, 8,
	2, 8, 4
};

Light::Light(vec3 _color) {
	color = _color;
	name = "ATVIEW_LIGHT";
	for (int i = 0; i < light_vertices.size() / 3; i++) {
		vec3 pos(light_vertices[i * 3 + 0], light_vertices[i * 3 + 1], light_vertices[i * 3 + 2]);
		Vertex vertex;
		vertex.position = pos * light_size;
		vertices.push_back(vertex);
	}
	minv = vec3(0.0);
	maxv = vec3(light_size);
	lenv = vec3(light_size);
	for (int i = 0; i < light_indices.size(); i++)
		indices.push_back(light_indices[i] - 1);

	Material material;
	material.diffuse = vec4(color, 1.0);
	materials.push_back(material);
}
