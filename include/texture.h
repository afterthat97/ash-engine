#ifndef TEXTURE_H
#define TEXTURE_H

#include "utilities.h"
#include "shader.h"

enum textureType { AMBIENT, DIFFUSE, SPECULAR, NORMAL };

class Texture {
private:
	uint32_t textureID, width, height;
	textureType mytype;
public:
	string name;
	Texture(textureType _mytype) {
		name = "Untitled Texture";
		mytype = _mytype;
		width = height = 0;
	}
	void loadFromFile(string, string);
	void bind(Shader&);
	void dumpinfo(string);
};

#endif
