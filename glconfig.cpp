#include "glconfig.h"

string shadeModelStr = "SMOOTH";
string polygonModeStr = "FILL";
bool enableDepthTest = true;
bool enableLight = true;
bool enableTexture = true;
bool enableCullFace = false;
bool enableMultiSample = true;

void TW_CALL setShadeModel(const void *value, void *clientData) {
	string* src = (string*) value, *des = (string*) clientData;
	*des = *src;
	if (*des != "SMOOTH" && *des != "FLAT") *des = "SMOOTH";
}

void TW_CALL getShadeModel(void *value, void *clientData) {
	string* des = (string*) value, *src = (string*) clientData;
	TwCopyStdStringToLibrary(*des, *src);
}

void TW_CALL setPolygonMode(const void *value, void *clientData) {
	string* src = (string*) value, *des = (string*) clientData;
	*des = *src;
	if (*des != "FILL" && *des != "LINE") *des = "FILL";
}

void TW_CALL getPolygonMode(void *value, void *clientData) {
	string* des = (string*) value, *src = (string*) clientData;
	TwCopyStdStringToLibrary(*des, *src);
}
