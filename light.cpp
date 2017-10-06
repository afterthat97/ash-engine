#include "light.h"

void Light::enable() {
	glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0 + id);
}

void Light::disable() {
	glDisable(GL_LIGHT0 + id);
}

void Light::loadDefault() {
	for (int32_t i = 0; i < 3; i++) ambient[i] = 1.0f;
	for (int32_t i = 0; i < 3; i++) diffuse[i] = 1.0f;
	for (int32_t i = 0; i < 3; i++) specular[i] = 1.0f;
	ambient[3] = diffuse[3] = specular[3] = 1.0f;
}
