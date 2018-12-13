#include <Generic/Gridline.h>

pair<float, float> Gridline::xRange(-1000.0f, 1000.0f);
pair<float, float> Gridline::zRange(-1000.0f, 1000.0f);
float Gridline::yValue = 0.0f;
float Gridline::stride = 50.0f;
QVector3D Gridline::color(0.4f, 0.4f, 0.4f);
Mesh* Gridline::gridlineMesh = NULL;

void Gridline::setXRange(float start, float end) {
    xRange = pair<float, float>(start, end);
    generateMesh();
}

void Gridline::setYValue(float value) {
    yValue = value;
    generateMesh();
}

void Gridline::setZRange(float start, float end) {
    zRange = pair<float, float>(start, end);
    generateMesh();
}

void Gridline::setStride(float _stride) {
    stride = _stride;
    generateMesh();
}

void Gridline::setColor(QVector3D _color) {
    color = _color;
    if (gridlineMesh == NULL)
        generateMesh();
    else
        gridlineMesh->getMaterial()->setDiffuseColor(color);
}

Mesh * Gridline::getGridlineMesh() {
    if (gridlineMesh == NULL) generateMesh();
    return gridlineMesh;
}

void Gridline::generateMesh() {
    if (gridlineMesh) delete gridlineMesh;

    vector<Vertex> vertices;
    vector<uint32_t> indices;

    for (float xValue = xRange.first; xValue < xRange.second + 0.001f; xValue += stride) {
        vertices.push_back(Vertex(QVector3D(xValue, yValue, zRange.first)));
        vertices.push_back(Vertex(QVector3D(xValue, yValue, zRange.second)));
        indices.push_back(vertices.size() - 2);
        indices.push_back(vertices.size() - 1);
    }

    for (float zValue = zRange.first; zValue < zRange.second + 0.001f; zValue += stride) {
        vertices.push_back(Vertex(QVector3D(xRange.first, yValue, zValue)));
        vertices.push_back(Vertex(QVector3D(xRange.second, yValue, zValue)));
        indices.push_back(vertices.size() - 2);
        indices.push_back(vertices.size() - 1);
    }

    Material* material = new Material;
    material->setDiffuseColor(color);

    gridlineMesh = new Mesh(Mesh::Line);
    gridlineMesh->setName("Gridline");
    gridlineMesh->setVertices(vertices);
    gridlineMesh->setIndices(indices);
    gridlineMesh->setMaterial(material);
}
