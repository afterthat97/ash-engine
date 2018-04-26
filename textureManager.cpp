#include "textureManager.h"

shared_ptr<Texture> TextureManager::loadTexture(string filePath, TextureType textureType) {
	for (uint32_t i = 0; i < loadedTextures.size(); i++)
		if (loadedTextures[i]->getPath() == filePath) {
			reportInfo("Texture " + loadedTextures[i]->getPath() + "has been re-used.");
			return loadedTextures[i];
		}
	shared_ptr<Texture> newTexture(new Texture(textureType));
	newTexture->loadFromFile(filePath, textureType == DIFFUSE);
	loadedTextures.push_back(newTexture);
	return newTexture;
}

void TextureManager::recycle() {
	for (uint32_t i = 0; i < loadedTextures.size(); i++)
		if (loadedTextures[i].use_count() == 1) {
			loadedTextures.erase(loadedTextures.begin() + i);
			--i;
		}
}
