#ifndef LIGHT_H
#define LIGHT_H

#include "shader.h"
#include "utilities.h"

class Light {
private:
	uint32_t VAO, VBO, EBO;
	vector<float> vertices;
	vector<uint32_t> indices;
public:
	vec3 pos, ambient, diffuse, specular;
	Light();
	void init();
	void render(Shader&);
};

#endif
