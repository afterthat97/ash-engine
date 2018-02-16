#ifndef SKYBOX_H
#define SKYBOX_H

#include "utilities.h"
#include "shader.h"
#include "texture.h"

class Skybox {
private:
	uint32_t VAO, VBO, EBO, textureID;
	vector<float> vertices;
	vector<uint32_t> indices;
public:
	Skybox() {
		VAO = VBO = EBO = 0;
	}
	void loadFromFile(vector<string>);
	void init();
	void render(Shader&);
};

#endif
