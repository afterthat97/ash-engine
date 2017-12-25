#ifndef LIGHT_H
#define LIGHT_H

#include "utilities.h"
#include "shader.h"

class Light {
private:
	uint32_t VAO, VBO, EBO;
	vector<float> vertices;
	vector<uint32_t> indices;
public:
	vec3 pos, ambient, diffuse, specular;
	Light() {
		VAO = VBO = EBO = 0;
		loadDefaultColor();
	}
	void loadDefaultColor();
	void init(float);
	void render(Shader&);
};

#endif
