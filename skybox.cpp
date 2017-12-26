#include "skybox.h"

// Cube

const float skybox_vertices[] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
};

const uint32_t skybox_indices[] = {
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

void Skybox::loadFromFile(vector<string> filenames) {
	FreeImage_Initialise(0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (uint32_t i = 0; i < filenames.size(); i++) {
		// Use FreeImage to load texture
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filenames[i].c_str(), 0);
		FIBITMAP *bitmap = FreeImage_Load(fifmt, filenames[i].c_str(), 0);
		if (bitmap == NULL)
			throwError("load", filenames[i], "Could not open file");
		bitmap = FreeImage_ConvertTo32Bits(bitmap);
		uint8_t *textureArr = (uint8_t*) FreeImage_GetBits(bitmap);

		// Get resolution
		uint32_t width = FreeImage_GetWidth(bitmap);
		uint32_t height = FreeImage_GetHeight(bitmap);

		// Create texture and generate mipmaps
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);

		// Clean
		FreeImage_Unload(bitmap);
		reportInfo("Skybox texture file " + filenames[i] + " loaded. (" + to_string(width) + " * " + to_string(height) + ")");
	}

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	FreeImage_DeInitialise();
}

void Skybox::init() {
	vertices.clear();
	indices.clear();
	for (int i = 0; i < 24; i++)
		vertices.push_back(skybox_vertices[i] - 0.5f);
	for (int i = 0; i < 36; i++)
		indices.push_back(skybox_indices[i] - 1);

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Skybox::render(Shader& shader) {
	glDepthMask(GL_FALSE);
	shader.use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}
