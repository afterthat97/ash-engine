#ifndef GLCONFIG_H
#define GLCONFIG_H

#include "utilities.h"

extern string polygonModeStr;

extern bool enableParallaxMap;
extern bool enableNormalMap;
extern bool enableDiffuseMap;
extern bool enableSpecularMap;

extern bool enableLight;
extern bool enableAttenuation;
extern bool enableShadow;
extern bool enableFaceCulling;
extern bool enableMultiSample;
extern bool enableGridlines;

void TW_CALL setPolygonMode(const void *value, void *clientData);
void TW_CALL getPolygonMode(void *value, void *clientData);

#endif
