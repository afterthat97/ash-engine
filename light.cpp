#include "light.h"

// Looks like a cube

const float light_vertices[24] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
};

const uint32_t light_indices[36] = {
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

void Light::loadDefaultColor() {
	// Load default light color
	ambient = vec3(0.2f, 0.2f, 0.2f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(1.0f, 1.0f, 1.0f);
}

void Light::init() {
	// A cube light
	vertices.clear();
	indices.clear();
	for (int i = 0; i < 24; i++)
		vertices.push_back(light_vertices[i] * 10);
	for (int i = 0; i < 36; i++)
		indices.push_back(light_indices[i] - 1);

	// Generate object ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind vertex array object
	glBindVertexArray(VAO);

	// Bind vertex buffer object
	if (vertices.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		glEnableVertexAttribArray(0);
	}
	
	// Bind element buffer object
	if (indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
	
	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Light::render(Shader& shader) {
	// Use and set Shader
	shader.use();
	shader.setVec3("light.ambient", ambient);
	shader.setVec3("light.diffuse", diffuse);
	shader.setVec3("light.specular", specular);

	// Render object
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
