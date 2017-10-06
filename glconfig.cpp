#include "glconfig.h"

int32_t shadeModel = GL_SMOOTH;
int32_t polygonMode = GL_FILL;
bool enableDepthTest = true;
bool enableLight = true;
bool enableTexture = true;
bool enableGridlines = true;

void toggleDepthTest() {
	enableDepthTest = !enableDepthTest;
}

void toggleLighting() {
	enableLight = !enableLight;
}

void toggleTexture() {
	enableTexture = !enableTexture;
}

void togglePolygonMode() {
	polygonMode = (polygonMode == GL_FILL ? GL_LINE : GL_FILL);
}

void toggleShadeModel() {
	shadeModel = (shadeModel == GL_FLAT ? GL_SMOOTH : GL_FLAT);
}

void toggleGridlines() {
	enableGridlines = !enableGridlines;
}
