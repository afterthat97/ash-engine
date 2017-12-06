#include "sdf.h"

SDF::~SDF() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void SDF::computeDistanceField(const vector<float>& meshVertices, uint32_t sample) {
	if (distance != NULL) freeArray3D(distance, nx, ny, nz);
	vec3 minv(FLT_MAX), maxv(-FLT_MAX), len;
	for (uint32_t i = 0; i < meshVertices.size(); i += 3) {
		minv.x = min(meshVertices[i + 0], minv.x);
		maxv.x = max(meshVertices[i + 0], maxv.x);
		minv.y = min(meshVertices[i + 1], minv.y);
		maxv.y = max(meshVertices[i + 1], maxv.y);
		minv.z = min(meshVertices[i + 2], minv.z);
		maxv.z = max(meshVertices[i + 2], maxv.z);
	}
	len = maxv - minv;
	minv -= len * vec3(0.25, 0.25, 0.25);
	maxv += len * vec3(0.25, 0.25, 0.25);
	len += len * vec3(0.5, 0.5, 0.5);
	float maxlen = max(len.x, max(len.y, len.z));
	delta = maxlen / sample;
	nx = (uint32_t)(len.x / delta);
	ny = (uint32_t)(len.y / delta);
	nz = (uint32_t)(len.z / delta);
	allocArray3D(distance, nx, ny, nz);
	fillArray3D(distance, nx, ny, nz, 0.0f);
	if (nx < 0 || ny < 0 || nz < 0) return;
	for (uint32_t i = 0; i < meshVertices.size(); i += 3) {
		float x = (meshVertices[i + 0] - minv.x) / delta;
		float y = (meshVertices[i + 1] - minv.y) / delta;
		float z = (meshVertices[i + 2] - minv.z) / delta;
		distance[(uint32_t)x][(uint32_t)y][(uint32_t)z] = 1.0f;
	}
	uint32_t ***tmp1 = NULL, ***tmp2 = NULL;
	allocArray3D(tmp1, nx, ny, nz);
	allocArray3D(tmp2, nx, ny, nz);
	reportInfo("Computing distance field, stage 1...");
	fillArray3D(tmp1, nx, ny, nz, (uint32_t)inf);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				for (uint32_t t = 0; t < nz; t++)
					if (distance[i][j][t] > 0.0f)
						tmp1[i][j][k] = min(tmp1[i][j][k], (t - k) * (t - k));
	reportInfo("Computing distance field, stage 2...");
	fillArray3D(tmp2, nx, ny, nz, (uint32_t)inf);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				for (uint32_t t = 0; t < ny; t++)
					tmp2[i][j][k] = min(tmp2[i][j][k], tmp1[i][t][k] + (t - j) * (t - j));
	reportInfo("Computing distance field, stage 3...");
	fillArray3D(distance, nx, ny, nz, FLT_MAX);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				for (uint32_t t = 0; t < nx; t++)
					distance[i][j][k] = min(distance[i][j][k], tmp2[t][j][k] + (t - i) * (t - i));
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++) {
				if (distance[i][j][k] < 20) {
					vertices.push_back(i * delta + minv.x); // X
					vertices.push_back(j * delta + minv.y); // Y
					vertices.push_back(k * delta + minv.z); // Z
					colors.push_back(1.0f / (distance[i][j][k] + 1)); // A
				}
				distance[i][j][k] = sqrt(distance[i][j][k]);
			}
	freeArray3D(tmp1, nx, ny, nz);
	freeArray3D(tmp2, nx, ny, nz);
}

void SDF::computeGradientVector() {
	if (distance == NULL) return;
	if (gradient != NULL) freeArray3D(gradient, nx, ny, nz);
	reportInfo("Computing gradient vector of distance field...");
	allocArray3D(gradient, nx, ny, nz);
	for (uint32_t i = 1; i < nx - 1; i++)
		for (uint32_t j = 1; j < ny - 1; j++)
			for (uint32_t k = 1; k < nz - 1; k++) {
				vec3 gradientVec(0.0f);
				gradientVec.x = distance[i - 1][j][k] - distance[i + 1][j][k];
				gradientVec.y = distance[i][j - 1][k] - distance[i][j + 1][k];
				gradientVec.z = distance[i][j][k - 1] - distance[i][j][k + 1];
				gradientVec = glm::normalize(gradientVec);
				gradient[i][j][k] = gradientVec;
			}
}

void SDF::initBO() {
	if (vertices.size() == 0) return;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &CBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	if (colors.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SDF::render(Shader& shader) {
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
	glBindVertexArray(0);
}