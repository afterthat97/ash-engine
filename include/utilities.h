#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <ApplicationServices/ApplicationServices.h>
#else
#include <GL/glut.h>
#include <Windows.h>
#define GL_BGRA GL_BGRA_EXT
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "FreeImage.lib")
#endif

#define fmin(x, y) ((x) < (y) ? (x) : (y))
#define fmax(x, y) ((x) > (y) ? (x) : (y))

using std::vector;
using std::string;
using std::pair;

#endif
