#ifndef GRIDLINES_H
#define GRIDLINES_H

#include "utilities.h"
#include "shader.h"
#include "mesh.h"

class Gridlines : public Mesh {
public:
    Gridlines();
    void render(Shader&);
};

#endif
