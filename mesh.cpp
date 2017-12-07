#include "mesh.h"

Mesh::~Mesh() {
	// Clean
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (NBO) glDeleteBuffers(1, &NBO);
	if (TBO) glDeleteBuffers(1, &TBO);
	if (UBO) glDeleteBuffers(1, &UBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Mesh::initBO() {
	// Generate object ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &UBO);
	glGenBuffers(1, &EBO);
	
	// Bind vertex array object
	glBindVertexArray(VAO);
	
	// Bind vertex buffer object
	if (vertices.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	// Bind normal buffer object
	if (normals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}
	
	// Bind tangent buffer object
	if (tangents.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size(), &tangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(2);
	}

	// Bind UV buffer object
	if (texCoords.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, UBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(3);
	}
	
	// Bind element buffer object
	if (indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
	
	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// Initialize SDF's buffer object
	sdf.initBO();
}

void Mesh::computeSDF(uint32_t sample) {
	sdf.computeDistanceField(vertices, sample);
	sdf.computeGradientVector();
}

vector<float> Mesh::pointSampling(uint32_t pointNum) {
	Mesh newMesh;
	newMesh.vertices = vertices;
	newMesh.indices = indices;

	// Compute the minimal triangle's area
	float minTriangleArea = FLT_MAX;
	for (uint32_t i = 0; i < indices.size();) {
		vec3 A = vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]); i++;
		vec3 B = vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]); i++;
		vec3 C = vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]); i++;
		vec3 AB = B - A, AC = C - A;
		minTriangleArea = fmin(minTriangleArea, glm::length(glm::cross(AB, AC)) / 2);
	}

	// Split large triangles info small triangles
	for (uint32_t i = 0; i < newMesh.indices.size();) {
		uint32_t j = i + 1, k = i + 2;
	
		// Triangle ABC
		vec3 A = vec3(newMesh.vertices[newMesh.indices[i] * 3],
			newMesh.vertices[newMesh.indices[i] * 3 + 1],
			newMesh.vertices[newMesh.indices[i] * 3 + 2]);
		vec3 B = vec3(newMesh.vertices[newMesh.indices[j] * 3],
			newMesh.vertices[newMesh.indices[j] * 3 + 1],
			newMesh.vertices[newMesh.indices[j] * 3 + 2]);
		vec3 C = vec3(newMesh.vertices[newMesh.indices[k] * 3],
			newMesh.vertices[newMesh.indices[k] * 3 + 1],
			newMesh.vertices[newMesh.indices[k] * 3 + 2]);
		
		// Compute the area of triangle
		vec3 AB = B - A, AC = C - A, BC = C - B;
		float area = glm::length(glm::cross(AB, AC)) / 2;
		if (area > 2.0f * minTriangleArea) {
			// Select the midpoint of the longest edge, and split the triangle into two pieces
			if (glm::length(AB) > glm::length(AC) && glm::length(AB) > glm::length(BC)) {
				// AB has max length
				vec3 M = (A + B) * vec3(0.5, 0.5, 0.5);
				uint32_t Mid = newMesh.vertices.size() / 3;
				newMesh.vertices.push_back(M.x);
				newMesh.vertices.push_back(M.y);
				newMesh.vertices.push_back(M.z);
				newMesh.indices.push_back(newMesh.indices[j]);
				newMesh.indices.push_back(newMesh.indices[k]);
				newMesh.indices.push_back(Mid);
				newMesh.indices[j] = Mid;
			} else if (glm::length(AC) > glm::length(AB) && glm::length(AC) > glm::length(BC)) {
				// AC has max length
				vec3 M = (A + C) * vec3(0.5, 0.5, 0.5);
				uint32_t Mid = newMesh.vertices.size() / 3;
				newMesh.vertices.push_back(M.x);
				newMesh.vertices.push_back(M.y);
				newMesh.vertices.push_back(M.z);
				newMesh.indices.push_back(newMesh.indices[j]);
				newMesh.indices.push_back(newMesh.indices[k]);
				newMesh.indices.push_back(Mid);
				newMesh.indices[k] = Mid;
			} else {
				// BC has max length
				vec3 M = (B + C) * vec3(0.5, 0.5, 0.5);
				uint32_t Mid = newMesh.vertices.size() / 3;
				newMesh.vertices.push_back(M.x);
				newMesh.vertices.push_back(M.y);
				newMesh.vertices.push_back(M.z);
				newMesh.indices.push_back(newMesh.indices[i]);
				newMesh.indices.push_back(newMesh.indices[k]);
				newMesh.indices.push_back(Mid);
				newMesh.indices[k] = Mid;
			}
		} else i += 3;
	}

	// Random point sampling
	srand(2017);
	uint32_t totVertices = newMesh.vertices.size() / 3;
	uint32_t *exist = new uint32_t[totVertices];
	vector<float> pointSample;

	// Sample too many points
	if (pointNum > totVertices / 2) {
		pointNum = totVertices / 2;
		reportWarning("Too many sample points. Only " + to_string(pointNum) + " points will be sampled.");
	}

	// Sampling
	for (uint32_t i = 0, vid; i < pointNum; i++) {
		while (exist[vid = rand() % totVertices] == 1);
		exist[vid] = 1;
		pointSample.push_back(newMesh.vertices[vid * 3 + 0]);
		pointSample.push_back(newMesh.vertices[vid * 3 + 1]);
		pointSample.push_back(newMesh.vertices[vid * 3 + 2]);
	}
	delete[] exist;
	return pointSample;
}

void Mesh::render(Shader& shader) {
	// Use shader
	shader.use();

	// Bind material
	for (uint32_t i = 0; i < materials.size(); i++) {
		materials[i].apply(shader);
		materials[i].bind(shader);
	}
	
	// Render object
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::renderSDF(Shader &shader) {
	sdf.render(shader);
}

void Mesh::dumpinfo(string tab) {
	printf("%sMesh %s:\n", tab.c_str(), name.c_str());
	printf("%s  Vertices: %lu\n", tab.c_str(), vertices.size() / 3);
	printf("%s  Triangles: %lu\n", tab.c_str(), indices.size() / 3);
	for (uint32_t i = 0; i < materials.size(); i++)
		materials[i].dumpinfo(tab + "  ");
}
