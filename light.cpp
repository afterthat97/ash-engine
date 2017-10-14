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

void Light::dumpinfo(string tab) {
	printf("%sLight %d, position: (%.1f %.1f %.1f %.1f).\n",
		tab.c_str(), id, position[0], position[1], position[2], position[3]);
	printf("%s  Ambient:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", ambient[i]);
	printf("\n");
	printf("%s  Diffuse:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", diffuse[i]);
	printf("\n");
	printf("%s  Specular:", tab.c_str());
	for (int32_t i = 0; i < 4; i++)
		printf(" %.2f", specular[i]);
	printf("\n");
}
