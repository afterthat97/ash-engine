#ifndef TEXTURE_H
#define TEXTURE_H

#include "utilities.h"
#include "shader.h"

enum TextureType { AMBIENT, DIFFUSE, SPECULAR, NORMAL, PARALLAX };

class Texture {
private:
    string path;
    uint32_t textureID, width, height;
	TextureType textureType;
	void loadFromFile(string filePath, bool SRGB);
public:
    Texture(TextureType _textureType);
    ~Texture();
	string getPath();
	uint32_t getWidth();
	uint32_t getHeight();
    void bind(Shader&);
    void dumpinfo(string);
	friend class TextureManager;
};

#endif
