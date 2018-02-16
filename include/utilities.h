#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cfloat>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#ifdef __APPLE__
	#include "TargetConditionals.h"
    #ifdef TARGET_OS_OSX
		#define APPLE_MACOS
		#include <ApplicationServices/ApplicationServices.h>
		#include <unistd.h>
		using std::min;
		using std::max;
	#else
		#error "platform not supported"
	#endif
#elif _WIN64
	#error "only win32 is supported"
#elif _WIN32
	#define MS_WIN32
	#include <Windows.h>
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "User32.lib")
	#pragma comment(lib, "glfw3dll.lib")
	#pragma comment(lib, "glew32.lib")
	#pragma comment(lib, "assimp.lib")
	#pragma comment(lib, "FreeImage.lib")
	#pragma comment(lib, "AntTweakBar.lib")
	#pragma comment(lib, "BulletCollision.lib")
	#pragma comment(lib, "BulletDynamics.lib")
	#pragma comment(lib, "BulletSoftBody.lib")
	#pragma comment(lib, "LinearMath.lib")
#else
	#error "platform not supported"
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <AntTweakBar.h>
#include <btBulletDynamicsCommon.h>

const int32_t inf = 0x3f3f3f3f;

using std::vector;
using std::string;
using std::to_string;
using std::pair;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::swap;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;
using glm::angle;

void throwError(string type, string name, string msg = "no error message");
void reportInfo(string);
void reportWarning(string);

string getFormatTime();
string getFormatDate();

vec3 minVec3(const vec3&, const vec3&);
vec3 maxVec3(const vec3&, const vec3&);

template <typename T>
void allocArray3D(T *** &arr, uint32_t x, uint32_t y, uint32_t z) {
	arr = new T**[x];
	for (uint32_t i = 0; i < x; i++) {
		arr[i] = new T*[y];
		for (uint32_t j = 0; j < y; j++)
			arr[i][j] = new T[z];
	}
}

template <typename T>
void fillArray3D(T *** &arr, uint32_t x, uint32_t y, uint32_t z, T val) {
	for (uint32_t i = 0; i < x; i++)
		for (uint32_t j = 0; j < y; j++)
			for (uint32_t k = 0; k < z; k++)
				arr[i][j][k] = val;
}

template <typename T>
void freeArray3D(T *** &arr, uint32_t x, uint32_t y, uint32_t z) {
	for (uint32_t i = 0; i < x; i++) {
		for (uint32_t j = 0; j < y; j++)
			delete[] arr[i][j];
		delete[] arr[i];
	}
	delete[] arr;
	arr = NULL;
}

#endif
