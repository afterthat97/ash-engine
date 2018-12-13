#pragma once

#include <Generic/Model.h>

class Gridline {
public:
    static void setXRange(float start, float end);
    static void setYValue(float value);
    static void setZRange(float start, float end);
    static void setStride(float stride);
    static void setColor(QVector3D color);
    static Mesh* getGridlineMesh();

private:
    static pair<float, float> xRange, zRange;
    static float yValue, stride;
    static QVector3D color;
    static Mesh* gridlineMesh;
    static void generateMesh();
};
