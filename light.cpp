#include "light.h"
#include "basicShapes.h"

Light::Light(vec3 _color, btDiscreteDynamicsWorld *_dynamicsWorld) : Mesh() {
    color = _color;
    dynamicsWorld = _dynamicsWorld;
    name = "ATVIEW_LIGHT";
    for (uint32_t i = 0; i < cube_vertices.size() / 3; i++) {
        vec3 pos(cube_vertices[i * 3 + 0], cube_vertices[i * 3 + 1], cube_vertices[i * 3 + 2]);
        Vertex vertex;
        vertex.position = pos * vec3(10.0);
        vertices.push_back(vertex);
    }
    minv = vec3(0.0); maxv = vec3(10.0); lenv = vec3(10.0);
    for (uint32_t i = 0; i < cube_indices.size(); i++)
        indices.push_back(cube_indices[i] - 1);

    shared_ptr<Material> newMaterial(new Material());
    newMaterial->diffuse = color;
    material = newMaterial;
    initBufferObject();
    initRigidBody();
    addToBulletDynamicsWorld();

    nearPlane = 1.0f; farPlane = 10000.0f;
    shadowResolution = 1024;
    depthCubeMap = depthMapFBO = 0;
}

Light::Light(const Light& a) : Mesh(a) {
    color = a.color;
    nearPlane = a.nearPlane;
    farPlane = a.farPlane;
    shadowResolution = a.shadowResolution;
    depthCubeMap = depthMapFBO = 0;
}

Light::~Light() {
    deleteDepthMap();
}

void Light::initDepthMap() {
    deleteDepthMap();

    // Generate depth map FBO
    glGenFramebuffers(1, &depthMapFBO);

    // Generate depth map texture
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::deleteDepthMap() {
    if (depthCubeMap) glDeleteTextures(1, &depthCubeMap);
    if (depthMapFBO) glDeleteFramebuffers(1, &depthMapFBO);
}

void Light::renderDepthMap(vector<Scene*> scenes, Shader& depthShader) {
    if (depthMapFBO == 0 || depthCubeMap == 0) initDepthMap();
    glViewport(0, 0, shadowResolution, shadowResolution);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthShader.use();
    mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
    depthShader.setMat4("shadowMatrices[0]", proj * glm::lookAt(pos, pos + vec3(1, 0, 0), vec3(0, -1, 0)));
    depthShader.setMat4("shadowMatrices[1]", proj * glm::lookAt(pos, pos + vec3(-1, 0, 0), vec3(0, -1, 0)));
    depthShader.setMat4("shadowMatrices[2]", proj * glm::lookAt(pos, pos + vec3(0, 1, 0), vec3(0, 0, 1)));
    depthShader.setMat4("shadowMatrices[3]", proj * glm::lookAt(pos, pos + vec3(0, -1, 0), vec3(0, 0, -1)));
    depthShader.setMat4("shadowMatrices[4]", proj * glm::lookAt(pos, pos + vec3(0, 0, 1), vec3(0, -1, 0)));
    depthShader.setMat4("shadowMatrices[5]", proj * glm::lookAt(pos, pos + vec3(0, 0, -1), vec3(0, -1, 0)));
    depthShader.setFloat("farPlane", farPlane);
    depthShader.setVec3("lightPos", pos);

    for (uint32_t i = 0; i < scenes.size(); i++)
        scenes[i]->render(depthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::setColor(vec3 _color) {
    color = _color;
    material->diffuse = color;
}

vec3 Light::getColor() {
    return color;
}

uint32_t Light::getDepthMap() {
    return depthCubeMap;
}

float Light::getFarPlane() {
    return farPlane;
}
