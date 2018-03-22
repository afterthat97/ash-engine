#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cfloat>
#include <ctime>
#include <memory>

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
using std::shared_ptr;

// GLM: Math library for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;
using glm::angle;
using glm::distance;
using glm::normalize;
using glm::cross;
using glm::dot;

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

#define MS_WIN64
#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "assimp-vc140-mt.lib")
#pragma comment(lib, "FreeImage.lib")
#pragma comment(lib, "AntTweakBar64.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "LinearMath.lib")

#elif _WIN32

#define MS_WIN32
#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "assimp-vc140-mt.lib")
#pragma comment(lib, "FreeImage.lib")
#pragma comment(lib, "AntTweakBar.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "LinearMath.lib")

#endif

// GLAD: OpenGL functions loader
#include <glad/glad.h>

// GLFW: a window manager
#include <GLFW/glfw3.h>

// FreeImage: texture loader
#include <FreeImage.h>

// Assimp: 3D model loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// AntTweakBar: side bar
#include <AntTweakBar.h>

// Bullet: physics engine
#include <btBulletDynamicsCommon.h>

vec3 toVec3(const aiColor4D& a);
vec3 toVec3(btVector3& a);
btVector3 toBtVec3(vec3& a);

void reportError(string msg);
void reportInfo(string msg);
void reportWarning(string msg);

string getFormatTime();
string getFormatDate();
string getFilePath(string dir, string filename);

vec3 minVec3(vec3, vec3);
vec3 maxVec3(vec3, vec3);

extern const vector<float> cube_vertices;

extern const vector<uint32_t> cube_indices;

extern const vector<float> arrow_vertices;

extern const vector<uint32_t> arrow_indices;

extern const vector<float> torus_vertices;

extern const vector<uint32_t> torus_indices;

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
