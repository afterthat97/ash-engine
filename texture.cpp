#include "texture.h"
#include "FreeImage/FreeImage.h"
#include "glconfig.h"

void Texture::loadFromFile(string dir, string filename) {
	FreeImage_Initialise(0);
	if (filename[0] == '/') filename.erase(0, 1);
	dir = dir + filename;
	name = filename;

	// Use FreeImage to load texture
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(dir.c_str(), 0);
	FIBITMAP *bitmap = FreeImage_Load(fifmt, dir.c_str(), 0);
	if (bitmap == NULL)
		throw ("Unable to load texture from " + dir).c_str();
	bitmap = FreeImage_ConvertTo32Bits(bitmap);
	uint8_t *textureArr = (uint8_t*)FreeImage_GetBits(bitmap);

	// Get resolution
	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// Clean
	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	reportInfo("Texture file " + filename + " loaded. (" + to_string(width) + " * " + to_string(height) + ")");
}

void Texture::bind(Shader& shader) {
	if (mytype == AMBIENT) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader.setInt("material.ambientMap", 0);
		shader.setInt("material.hasAmbientMap", textureID && enableTexture ? 1 : 0);
	} else if (mytype == DIFFUSE) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader.setInt("material.diffuseMap", 1);
		shader.setInt("material.hasDiffuseMap", textureID && enableTexture ? 1 : 0);
	} else if (mytype == SPECULAR) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader.setInt("material.specularMap", 2);
		shader.setInt("material.hasSpecularMap", textureID && enableTexture ? 1 : 0);
	} else if (mytype == NORMAL) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader.setInt("material.normalMap", 3);
		shader.setInt("material.hasNormalMap", textureID && enableTexture ? 1 : 0);
	}
}

void Texture::dumpinfo(string tab) {
	printf("%sTexture %s\n", tab.c_str(), name.c_str());
	if (mytype == AMBIENT)
		printf("%s  Type: Ambient\n", tab.c_str());
	else if (mytype == DIFFUSE)
		printf("%s  Type: Diffuse\n", tab.c_str());
	else if (mytype == SPECULAR)
		printf("%s  Type: Specular\n", tab.c_str());
	else if (mytype == NORMAL)
		printf("%s  Type: Normal\n", tab.c_str());
	printf("%s  Texture ID: %u\n", tab.c_str(), textureID);
	printf("%s  Resolution: %u * %u\n", tab.c_str(), width, height);
}
