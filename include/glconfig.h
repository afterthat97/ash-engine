#ifndef GLCONFIG_H
#define GLCONFIG_H

#include "utilities.h"
#include <AntTweakBar/AntTweakBar.h>

extern int32_t shadeModel;
extern int32_t polygonMode;
extern string shadeModelStr;
extern string polygonModeStr;
extern bool enableDepthTest;
extern bool enableLight;
extern bool enableTexture;
extern bool enableCullFace;
extern bool enableMultiSample;

void TW_CALL setShadeModel(const void *value, void *clientData);
void TW_CALL setPolygonMode(const void *value, void *clientData);
void TW_CALL getShadeModel(void *value, void *clientData);
void TW_CALL getPolygonMode(void *value, void *clientData);

#endif
