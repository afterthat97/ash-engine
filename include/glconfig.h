#ifndef GLCONFIG_H
#define GLCONFIG_H

#include "utilities.h"

struct Window {
	string title;
	int32_t width;
	int32_t height;
	Window() {
		title = "3DViewer";
		width = 1600;
		height = 900;
	}
};

extern int32_t shadeModel;
extern int32_t polygonMode;
extern bool enableDepthTest;
extern bool enableLight;
extern bool enableTexture;
extern bool enableGridlines;

void applyConfig();
void toggleDepthTest();
void toggleLighting();
void toggleTexture();
void togglePolygonMode();
void toggleGridlines();
void toggleShadeModel();

#endif
