#include "gridlines.h"

Gridlines::Gridlines() : Mesh() {
    float length = 1000.0, width = 1000.0, inc = 50.0;
	
    // Generate vertices and indices for rendering
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
    
    // Size
    minv = vec3(-length, -width, 0);
    maxv = vec3(length, width, 0);
    lenv = maxv - minv;
    
    // Materials (grey: 0.4, 0.4, 0.4)
    shared_ptr<Material> newMaterial(new Material());
    newMaterial->diffuse = vec3(0.4f);
    material = newMaterial;
    
    // Initialize VAO, VBO, etc
    initBufferObject();
}

// Render gridlines
void Gridlines::render(Shader& shader) {
    // Use and configure shader
    shader.use();
    shader.setMat4("model", model);
    shader.setInt("selected", 0);

    // Bind material
    material->bind(shader);

    // Render
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
