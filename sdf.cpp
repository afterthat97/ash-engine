#include "sdf.h"

SDF::~SDF() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void SDF::compute(const vector<float> vertices, float delta) {
	
}

void SDF::render() {
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, , GL_UNSIGNED_INT, 0);
}
