#include "texture.h"
#include "glconfig.h"

bool Texture::loadFromFile(string dir, string filename) {
    FreeImage_Initialise(0);
    if (filename[0] == '/') filename.erase(0, 1);
    for (uint32_t i = 0; i < filename.size(); i++)
        if (filename[i] == '\\') filename[i] = '/';
    dir = dir + filename;
    name = filename;

    // Use FreeImage to load texture
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(dir.c_str(), 0);
    FIBITMAP *bitmap = FreeImage_Load(fifmt, dir.c_str(), 0);
    if (bitmap == NULL) {
        reportError("Failed to load " + dir);
        return false;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean
    glBindTexture(GL_TEXTURE_2D, 0);
    FreeImage_Unload(bitmap);
    FreeImage_Unload(bitmap32);
    FreeImage_DeInitialise();
    reportInfo("Texture " + filename + " loaded. (" + to_string(width) + " * " + to_string(height) + ")");
    return true;
}

void Texture::bind(Shader& shader) {
    if (mytype == AMBIENT) { // Ambient map
        // Not support
    } else if (mytype == DIFFUSE) { // Diffuse map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.diffuseMap", 1);
        shader.setBool("material.hasDiffuseMap", textureID && enableDiffuseMap ? 1 : 0);
    } else if (mytype == SPECULAR) { // Specular map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.specularMap", 2);
        shader.setBool("material.hasSpecularMap", textureID && enableSpecularMap ? 1 : 0);
    } else if (mytype == NORMAL) { // Normal map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.normalMap", 3);
        shader.setBool("material.hasNormalMap", textureID && enableNormalMap ? 1 : 0);
    } else if (mytype == PARALLAX) { // Parallax map
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.parallaxMap", 4);
        shader.setBool("material.hasParallaxMap", textureID && enableParallaxMap ? 1 : 0);
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
    else if (mytype == PARALLAX)
        printf("%s  Type: Parallax\n", tab.c_str());
    printf("%s  Texture ID: %u\n", tab.c_str(), textureID);
    printf("%s  Resolution: %u * %u\n", tab.c_str(), width, height);
}
