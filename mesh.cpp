#include "mesh.h"
#include "glconfig.h"

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
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	
	glBindVertexArray(0);
}	

void Mesh::render(Shader& shader) {
	// Use shader
	shader.use();
	shader.setMat4("model", mat4(1.0f));

	// Bind material
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].bind(shader);
	
	// Render object
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::dumpinfo(string tab) {
	printf("%sMesh %s:\n", tab.c_str(), name.c_str());
	printf("%s  Vertices: %lu\n", tab.c_str(), vertices.size());
	printf("%s  Triangles: %lu\n", tab.c_str(), indices.size() / 3);
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].dumpinfo(tab + "  ");
}
