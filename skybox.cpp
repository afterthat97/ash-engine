#include "skybox.h"
#include "basicShapes.h"

Skybox::Skybox() : Mesh() {
    cubeTextureID = 0;
	
    // Vertices and indices
    for (uint32_t i = 0; i < cube_vertices.size() / 3; i++) {
        vec3 pos(cube_vertices[i * 3 + 0], cube_vertices[i * 3 + 1], cube_vertices[i * 3 + 2]);
        Vertex vertex;
        vertex.position = pos - vec3(0.5f);
        vertices.push_back(vertex);
    }
    for (uint32_t i = 0; i < cube_indices.size(); i++)
        indices.push_back(cube_indices[i] - 1);

    // Initialize VAO, VBO, etc
    initBufferObject();
}

// Load cube map from 6 image files
void Skybox::loadCubeMap(vector<string> filenames) {
    FreeImage_Initialise(0);
    glGenTextures(1, &cubeTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);

    for (uint32_t i = 0; i < filenames.size(); i++) {
        // Use FreeImage to load texture
        FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filenames[i].c_str(), 0);
        FIBITMAP *bitmap = FreeImage_Load(fifmt, filenames[i].c_str(), 0);
        if (bitmap == NULL) {
            cubeTextureID = 0;
            FreeImage_DeInitialise();
            reportError("Failed to load " + filenames[i]);
            return;
        }
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

// Render
void Skybox::render(Shader& shader) {
    if (cubeTextureID != 0) {
        // Use and configure shader
        shader.use();
        shader.setInt("hasCubeMap", 1);
        shader.setInt("cubeMap", 0);
        
        // Call OpenGL functions to render skybox
        glDepthMask(GL_FALSE);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }
}
