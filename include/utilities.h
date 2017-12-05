#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>
#else
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")
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
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;

void throwError(string type, string name, string msg = "no error message");
void reportInfo(string);

#endif
