#include "texture.h"
#include "glconfig.h"

Texture::Texture(TextureType _textureType) {
	textureType = _textureType;
	textureID = width = height = 0;
}

Texture::~Texture() {
	if (textureID) {
		glDeleteTextures(1, &textureID);
		reportInfo("Texture " + path + " has been unloaded.");
	}
}

// Load texture from image file
void Texture::loadFromFile(string filePath, bool SRGB) {
    FreeImage_Initialise(0);
    path = filePath;

    // Use FreeImage to load texture
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(path.c_str(), 0);
    FIBITMAP *bitmap = FreeImage_Load(fifmt, path.c_str(), 0);
    if (bitmap == NULL) {
        reportError("Failed to load " + path);
		FreeImage_DeInitialise();
        return;
    }
    FIBITMAP *bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    uint8_t *textureArr = (uint8_t*) FreeImage_GetBits(bitmap32);

    // Get resolution
    width = FreeImage_GetWidth(bitmap32);
    height = FreeImage_GetHeight(bitmap32);

    // Generate texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, SRGB ? GL_SRGB : GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean
    glBindTexture(GL_TEXTURE_2D, 0);
    FreeImage_Unload(bitmap);
    FreeImage_Unload(bitmap32);
    FreeImage_DeInitialise();
    reportInfo("Texture " + path + " loaded. (" + to_string(width) + " * " + to_string(height) + ")");
}

// Get the path of the original image file
string Texture::getPath() {
	return path;
}

// Return 1920 if the resolution is 1920x1080
uint32_t Texture::getWidth() {
	return width;
}

// Return 1080 if the resolution is 1920x1080
uint32_t Texture::getHeight() {
	return height;
}

// Bind texture for rendering
void Texture::bind(Shader& shader) {
	if (textureID == 0) return;
    if (textureType == AMBIENT) { // Ambient map
        // Not support
    } else if (textureType == DIFFUSE) { // Diffuse map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.diffuseMap", 1);
        shader.setBool("material.hasDiffuseMap", textureID && enableDiffuseMap && !enableWireFrame);
    } else if (textureType == SPECULAR) { // Specular map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.specularMap", 2);
        shader.setBool("material.hasSpecularMap", textureID && enableSpecularMap && !enableWireFrame);
    } else if (textureType == NORMAL) { // Normal map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.normalMap", 3);
        shader.setBool("material.hasNormalMap", textureID && enableNormalMap && !enableWireFrame);
    } else if (textureType == PARALLAX) { // Parallax map
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.parallaxMap", 4);
        shader.setBool("material.hasParallaxMap", textureID && enableParallaxMap && !enableWireFrame);
    }
}

// Dump details to console
void Texture::dumpinfo(string tab) {
    printf("%sTexture %s\n", tab.c_str(), path.c_str());
    if (textureType == AMBIENT)
        printf("%s  Type: Ambient\n", tab.c_str());
    else if (textureType == DIFFUSE)
        printf("%s  Type: Diffuse\n", tab.c_str());
    else if (textureType == SPECULAR)
        printf("%s  Type: Specular\n", tab.c_str());
    else if (textureType == NORMAL)
        printf("%s  Type: Normal\n", tab.c_str());
    else if (textureType == PARALLAX)
        printf("%s  Type: Parallax\n", tab.c_str());
    printf("%s  Texture ID: %u\n", tab.c_str(), textureID);
    printf("%s  Resolution: %u * %u\n", tab.c_str(), width, height);
}

