#include "glconfig.h"

string polygonModeStr = "FILL";

bool enableDiffuseMap = true;
bool enableSpecularMap = true;
bool enableNormalMap = true;
bool enableParallaxMap = true;

bool enableLight = true;
bool enableAttenuation = true;
bool enableShadow = false;
bool enableFaceCulling = true;
bool enableMultiSample = false;
bool enableGridlines = true;
bool enableGlobalAxis = true;

void TW_CALL setPolygonMode(const void *value, void *clientData) {
    string* src = (string*) value, *des = (string*) clientData;
    *des = *src;
    if (*des != "FILL" && *des != "LINE") *des = "FILL";
}

void TW_CALL getPolygonMode(void *value, void *clientData) {
    string* des = (string*) value, *src = (string*) clientData;
    TwCopyStdStringToLibrary(*des, *src);
}
