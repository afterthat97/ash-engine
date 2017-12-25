#include "axis.h"
#include "extmath.h"

// Arrow

const float arrow_vertices[42] = {
	55, 0, 0,
	55, 0, -0.5,
	50, 1, -0.5,
	50, 1, 0,
	50, -1.5, -0.5,
	50, -1.5, 0,
	50, 0, 0,
	50, 0, -0.5,
	50, -0.5, 0,
	50, -0.5, -0.5,
	0, 0, -0.5,
	0, 0, 0,
	0, -0.5, -0.5,
	0, -0.5, 0
};

const uint32_t arrow_indices[60] = {
	3, 5, 6,
	3, 4, 6,
	3, 4, 1,
	3, 2, 1,
	6, 5, 2,
	6, 1, 2,
	4, 6, 1,
	2, 5, 3,
	7, 9, 10,
	7, 8, 10,
	11, 13, 14,
	11, 12, 14,
	11, 12, 7,
	11, 8, 7,
	14, 13, 10,
	14, 9, 10,
	12, 14, 9,
	12, 7, 9,
	8, 10, 13,
	8, 11, 13
};

void Axis::init(float scale) {
	vertices.clear();
	indices.clear();
	for (uint32_t i = 0; i < 42; i++)
		vertices.push_back(arrow_vertices[i] * scale / 50.0f);
	for (uint32_t i = 0; i < 60; i++)
		indices.push_back(arrow_indices[i] - 1);
	
	// Generate object ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind vertex array object
	glBindVertexArray(VAO);

	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	
	// Bind element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);
	
	// Unbind
	glBindVertexArray(0);
}

void Axis::render(Shader& shader) {
	mat4 model(1.0f);

	// Use shader
	shader.use();

	// Render Axis X
	shader.setMat4("model", model);
	shader.setVec4("objColor", vec4(0.8, 0.0, 0.0, 1.0));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Render Axis Y
	shader.setMat4("model", mat4(rotate(vec3(0.0, 0.0, 1.0), M_PI / 2)) * model);
	shader.setVec4("objColor", vec4(0.0, 0.8, 0.0, 1.0));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	// Render Axis Z
	shader.setMat4("model", mat4(rotate(vec3(0.0, -1.0, 0.0), M_PI / 2)) * model);
	shader.setVec4("objColor", vec4(0.0, 0.0, 0.8, 1.0));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
