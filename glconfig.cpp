#include "glconfig.h"

bool enableDiffuseMap = true;
bool enableSpecularMap = true;
bool enableNormalMap = true;
bool enableParallaxMap = true;

bool enableLighting = true;
bool enableDoubleSideLighting = false;
bool enableAttenuation = true;
float attenuation_quadratic = 0.000007f;
float attenuation_linear = 0.0014f;
float attenuation_constant = 1.0f;

bool enableShadow = false;
uint16_t shadowResolution = 1024;
float bias = 0.5f;
float radius = 0.5f;

bool enableFaceCulling = true;
bool enableGridlines = true;
bool enableVSync = true;
bool enableMSAA4X = true;
bool enableGammaCorrection = true;
bool enableWireFrame = false;
