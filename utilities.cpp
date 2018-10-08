#include "utilities.h"

// Convert aiColor4D (assimp) to vec3 (glm)
vec3 toVec3(const aiColor4D & a) {
    return vec3(a.r, a.g, a.b);
}

// Convert btVector3 (bullet) to vec3 (glm)
vec3 toVec3(btVector3& a) {
    return vec3(a[0], a[1], a[2]);
}

// Convert vec3 (glm) to btVector3 (bullet)
btVector3 toBtVec3(vec3& a) {
    return btVector3(a.x, a.y, a.z);
}

// Dump error messages to console
void reportError(string msg) {
    cout << "ERROR:   " << msg << endl;
}

// Dump details to console
void reportInfo(string msg) {
    cout << "INFO:    " << msg << endl;
}

// Dump warnings to console
void reportWarning(string msg) {
    cout << "WARNING: " << msg << endl;
}

// 2000-01-01
string getFormatDate() {
    time_t rawtime;
    time(&rawtime);
    tm *timeinfo = localtime(&rawtime);
    return to_string(timeinfo->tm_year + 1900)
        + "-" + to_string(timeinfo->tm_mon + 1)
        + "-" + to_string(timeinfo->tm_mday);
}

// 10.10.56
string getFormatTime() {
    time_t rawtime;
    time(&rawtime);
    tm *timeinfo = localtime(&rawtime);
    return to_string(timeinfo->tm_hour)
        + "." + to_string(timeinfo->tm_min)
        + "." + to_string(timeinfo->tm_sec);
}

// Fix the different representation of filepath on Windows and macOS
string getFilePath(string dir, string filename) {
    if (filename[0] == '/') filename.erase(0, 1);
    for (uint32_t i = 0; i < filename.size(); i++)
        if (filename[i] == '\\') filename[i] = '/';
    return dir + filename;
}

// Load a text file to a string, lines are separated by '\n'
string loadStringFromFile(string filepath) {
    if (filepath == "") return "";
    ifstream fin(filepath);
    if (!fin.is_open()) {
        reportError("Failed to open " + filepath);
        return "";
    }
    string str;
    for (string line; getline(fin, line);)
        str += "\n" + line;
    fin.close();
    return str;
}

vec3 minVec3(vec3 v1, vec3 v2) {
    return vec3(min(v1[0], v2[0]), min(v1[1], v2[1]), min(v1[2], v2[2]));
}

vec3 maxVec3(vec3 v1, vec3 v2) {
    return vec3(max(v1[0], v2[0]), max(v1[1], v2[1]), max(v1[2], v2[2]));
}

// Functions for debug
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
