#ifndef VERTEX_H
#define VERTEX_H

#include "utilities.h"

class Vertex {
public:
    vec3 position, normal, tangent, bitangent;
    vec2 texCoord;
    Vertex() {
        position = normal = tangent = bitangent = vec3(1.0f);
        texCoord = vec2(0.0f);
    }
    Vertex(vec3 _pos) {
        position = _pos;
        normal = tangent = bitangent = vec3(1.0f);
        texCoord = vec2(1.0f);
    }
};

#endif
