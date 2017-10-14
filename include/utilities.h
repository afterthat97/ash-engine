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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ApplicationServices/ApplicationServices.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "FreeImage.lib")
#pragma comment(lib, "AntTweakBar.lib")
#endif

#define fmin(x, y) ((x) < (y) ? (x) : (y))
#define fmax(x, y) ((x) > (y) ? (x) : (y))

using std::vector;
using std::string;
using std::to_string;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;

#endif
