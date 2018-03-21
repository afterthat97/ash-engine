#ifndef SKYBOX_H
#define SKYBOX_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

class Skybox : public Mesh {
private:
    uint32_t cubeTextureID;
public:
    Skybox();
    void loadCubeMap(vector<string> filenames);
    void render(Shader&);
};

#endif
