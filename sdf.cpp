#include "sdf.h"

SDF::~SDF() {
	// Clean
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (CBO) glDeleteBuffers(1, &CBO);
}

void SDF::loadFromFile(const string& filename) {
	if (distance != NULL) freeArray3D(distance, nx, ny, nz);

	// Open SDF file
	ifstream fin(filename);
	if (!fin.is_open())
		throwError("load", filename, "Could not open file");
	string head; fin >> head;
	if (head != "SDF")
		throwError("load", filename, "File type not match");
	fin >> nx >> ny >> nz;
	swap(nx, nz);
	fin >> minv.x >> minv.y >> minv.z;
	fin >> delta;
	maxv.x = minv.x + delta * (nx - 1);
	maxv.y = minv.y + delta * (ny - 1);
	maxv.z = minv.z + delta * (nz - 1);
	len = maxv - minv;
	
	// Load 3D SDF from file
	allocArray3D(distance, nx, ny, nz);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++) {
				fin >> distance[i][j][k];
				if (distance[i][j][k] < 0 || distance[i][j][k] > 1.0f)
					distance[i][j][k] = FLT_MAX;
			}
	reportInfo("SDF file " + filename + " loaded.");
}

void SDF::computeDistanceField(const vector<float>& meshVertices, uint32_t sample) {
	if (distance != NULL) freeArray3D(distance, nx, ny, nz);

	// Get boundry
	minv = vec3(FLT_MAX); maxv = vec3(-FLT_MAX);
	for (uint32_t i = 0; i < meshVertices.size(); i += 3) {
		minv.x = fmin(meshVertices[i + 0], minv.x);
		maxv.x = fmax(meshVertices[i + 0], maxv.x);
		minv.y = fmin(meshVertices[i + 1], minv.y);
		maxv.y = fmax(meshVertices[i + 1], maxv.y);
		minv.z = fmin(meshVertices[i + 2], minv.z);
		maxv.z = fmax(meshVertices[i + 2], maxv.z);
	}
	len = maxv - minv;

	// Expand the boundry for distance field
	minv -= len * vec3(0.25, 0.25, 0.25);
	maxv += len * vec3(0.25, 0.25, 0.25);
	len += len * vec3(0.5, 0.5, 0.5);
	float maxlen = max(len.x, max(len.y, len.z));

	// Size of grid
	delta = maxlen / sample;
	nx = (uint32_t) (len.x / delta);
	ny = (uint32_t) (len.y / delta);
	nz = (uint32_t) (len.z / delta);

	// Map the mesh into distance field
	allocArray3D(distance, nx, ny, nz);
	fillArray3D(distance, nx, ny, nz, 0.0f);
	for (uint32_t i = 0; i < meshVertices.size(); i += 3) {
		float x = (meshVertices[i + 0] - minv.x) / delta;
		float y = (meshVertices[i + 1] - minv.y) / delta;
		float z = (meshVertices[i + 2] - minv.z) / delta;
		distance[(uint32_t)x][(uint32_t)y][(uint32_t)z] = 1.0f;
	}

	// Allocate memory space
	uint32_t ***tmp1 = NULL, ***tmp2 = NULL;
	allocArray3D(tmp1, nx, ny, nz);
	allocArray3D(tmp2, nx, ny, nz);

	// Stage 1: shortest distance in Z axis
	reportInfo("Computing distance field, stage 1...");
	fillArray3D(tmp1, nx, ny, nz, (uint32_t)inf);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				for (uint32_t t = 0; t < nz; t++)
					if (distance[i][j][t] > 0.0f)
						tmp1[i][j][k] = min(tmp1[i][j][k], (t - k) * (t - k));
	
	// Stage 2: shortest distance in Y and Z axis
	reportInfo("Computing distance field, stage 2...");
	fillArray3D(tmp2, nx, ny, nz, (uint32_t)inf);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				for (uint32_t t = 0; t < ny; t++)
					tmp2[i][j][k] = min(tmp2[i][j][k], tmp1[i][t][k] + (t - j) * (t - j));

	// Stage 3: shortest distance in X, Y and Z axis
	reportInfo("Computing distance field, stage 3...");
	fillArray3D(distance, nx, ny, nz, FLT_MAX);
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				for (uint32_t t = 0; t < nx; t++)
					distance[i][j][k] = min(distance[i][j][k], (float) tmp2[t][j][k] + (t - i) * (t - i));

	// Square
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				distance[i][j][k] = sqrt(distance[i][j][k]);

	// Clean
	freeArray3D(tmp1, nx, ny, nz);
	freeArray3D(tmp2, nx, ny, nz);
}

void SDF::computeGradientVector() {
	if (distance == NULL) return;
	if (gradient != NULL) freeArray3D(gradient, nx, ny, nz);

	// Compute the gradient vector for each grid in the distance field
	reportInfo("Computing gradient vector of distance field...");
	allocArray3D(gradient, nx, ny, nz);
	for (uint32_t i = 1; i < nx - 1; i++)
		for (uint32_t j = 1; j < ny - 1; j++)
			for (uint32_t k = 1; k < nz - 1; k++) {
				vec3 gradientVec(0.0f);
				gradientVec.x = distance[i - 1][j][k] - distance[i + 1][j][k];
				gradientVec.y = distance[i][j - 1][k] - distance[i][j + 1][k];
				gradientVec.z = distance[i][j][k - 1] - distance[i][j][k + 1];

				// Normalize the vector
				gradientVec = glm::normalize(gradientVec);
				gradient[i][j][k] = gradientVec;
			}
}

void SDF::initBO() {
	// Prepare for rendering
	for (uint32_t i = 0; i < nx; i++)
		for (uint32_t j = 0; j < ny; j++)
			for (uint32_t k = 0; k < nz; k++)
				if (distance[i][j][k] < 20) {
					vertices.push_back(i * delta + minv.x); // X
					vertices.push_back(j * delta + minv.y); // Y
					vertices.push_back(k * delta + minv.z); // Z
					colors.push_back(1.0f / (distance[i][j][k] + 1)); // A
				}
	// Generate object ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &CBO);

	// Bind vertex array object
	glBindVertexArray(VAO);

	// Bind vertex buffer object
	if (vertices.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	// Bind color buffer object
	if (colors.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SDF::render(Shader& shader) {
	// Use shader
	shader.use();
	
	// Render object
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
	glBindVertexArray(0);
}
