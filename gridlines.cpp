#include "gridlines.h"

void Gridlines::init(float length, float width, float inc) {
	vertices.clear();
	for (float x = -length; x < length; x += inc) {
		vertices.push_back(x);
		vertices.push_back(0.0f);
		vertices.push_back(-width);

		vertices.push_back(x);
		vertices.push_back(0.0f);
		vertices.push_back(width);

		vertices.push_back(-length);
		vertices.push_back(0.0f);
		vertices.push_back(x);

		vertices.push_back(length);
		vertices.push_back(0.0f);
		vertices.push_back(x);
	}
	// Generate object ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	// Bind vertex array object
	glBindVertexArray(VAO);

	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	
	// Unbind
	glBindVertexArray(0);
}

void Gridlines::render(Shader& shader) {
	shader.use();
	shader.setMat4("model", model);
	shader.setVec4("objColor", vec4(vec3(0.55), 1.0));
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertices.size() / 3);
	glBindVertexArray(0);
}
