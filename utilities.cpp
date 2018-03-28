#include "utilities.h"

vec3 toVec3(const aiColor4D & a) {
    return vec3(a.r, a.g, a.b);
}

vec3 toVec3(btVector3& a) {
    return vec3(a[0], a[1], a[2]);
}

btVector3 toBtVec3(vec3& a) {
    return btVector3(a.x, a.y, a.z);
}

void reportError(string msg) {
    cout << "ERROR:   " << msg << endl;
}

void reportInfo(string msg) {
    cout << "INFO:    " << msg << endl;
}

void reportWarning(string msg) {
    cout << "WARNING: " << msg << endl;
}

string getFormatDate() {
    time_t rawtime;
    time(&rawtime);
    tm *timeinfo = localtime(&rawtime);
    return to_string(timeinfo->tm_year + 1900)
        + "-" + to_string(timeinfo->tm_mon + 1)
        + "-" + to_string(timeinfo->tm_mday);
}

string getFormatTime() {
    time_t rawtime;
    time(&rawtime);
    tm *timeinfo = localtime(&rawtime);
    return to_string(timeinfo->tm_hour)
        + "." + to_string(timeinfo->tm_min)
        + "." + to_string(timeinfo->tm_sec);
}

string getFilePath(string dir, string filename) {
    if (filename[0] == '/') filename.erase(0, 1);
    for (uint32_t i = 0; i < filename.size(); i++)
        if (filename[i] == '\\') filename[i] = '/';
    return dir + filename;
}

vec3 minVec3(vec3 v1, vec3 v2) {
    return vec3(min(v1[0], v2[0]), min(v1[1], v2[1]), min(v1[2], v2[2]));
}

vec3 maxVec3(vec3 v1, vec3 v2) {
    return vec3(max(v1[0], v2[0]), max(v1[1], v2[1]), max(v1[2], v2[2]));
}

void glCheckError() {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               error = "Unknown error"; break;
        }
        reportError("OpenGL internal error: " + error);
    }
}
