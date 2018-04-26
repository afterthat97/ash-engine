#ifndef GLCONFIG_H
#define GLCONFIG_H

#include "utilities.h"

extern bool enableParallaxMap;
extern bool enableNormalMap;
extern bool enableDiffuseMap;
extern bool enableSpecularMap;

extern bool enableShadow;
extern uint16_t shadowResolution;
extern float bias;
extern float radius;

extern bool enableLight;
extern bool enableAttenuation;
extern bool enableDoubleSide;
extern bool enableFaceCulling;
extern bool enableGridlines;
extern bool enableVSync;
extern bool enableMSAA4X;
extern bool enableGammaCorrection;
extern bool enableWireFrame;

#endif
