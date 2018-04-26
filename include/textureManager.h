#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "utilities.h"
#include "shader.h"
#include "texture.h"

class TextureManager {
private:
	vector<shared_ptr<Texture>> loadedTextures;
public:
	TextureManager() {}
	shared_ptr<Texture> loadTexture(string filePath, TextureType textureType);
	void recycle();
};

#endif
