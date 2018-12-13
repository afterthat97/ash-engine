#pragma once

#include <OpenGL/OpenGLMesh.h>
#include <OpenGL/OpenGLTexture.h>
#include <Generic/Mesh.h>
#include <Generic/Texture.h>

class OpenGLManager {
public:
    static void cleanOpenGLMesh();
    static void cleanOpenGLTexture();
    static OpenGLMesh* getOpenGLMesh(Mesh* mesh);
    static OpenGLTexture* getOpenGLTexture(Texture *texture);

private:
    static map<uint32_t, OpenGLMesh*> mesh_map;
    static map<uint32_t, OpenGLTexture*> texture_map;
};