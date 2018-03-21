#include "gridlines.h"

Gridlines::Gridlines() : Mesh() {
    float length = 1000.0, width = 1000.0, inc = 50.0;
    for (float x = -length; x <= length; x += inc) {
        indices.push_back((uint32_t) vertices.size());
        vertices.push_back(Vertex(vec3(x, 0, -width)));
        indices.push_back((uint32_t) vertices.size());
        vertices.push_back(Vertex(vec3(x, 0, width)));
        indices.push_back((uint32_t) vertices.size());
        vertices.push_back(Vertex(vec3(-length, 0, x)));
        indices.push_back((uint32_t) vertices.size());
        vertices.push_back(Vertex(vec3(length, 0, x)));
    }
    minv = vec3(-length, -width, 0);
    maxv = vec3(length, width, 0);
    lenv = maxv - minv;
    shared_ptr<Material> newMaterial(new Material());
    newMaterial->diffuse = vec3(0.4f);
    material = newMaterial;
    initBufferObject();
}

void Gridlines::render(Shader& shader) {
    shader.use();
    shader.setMat4("model", model);
    shader.setInt("selected", 0);

    material->bind(shader);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
