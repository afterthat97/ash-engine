#include <OpenGL/OpenGLManager.h>
#include <OpenGL/OpenGLConfig.h>

map<uint32_t, OpenGLMesh*> OpenGLManager::mesh_map;
map<uint32_t, OpenGLTexture*> OpenGLManager::texture_map;

void OpenGLManager::cleanOpenGLMesh() {
    for (map<uint32_t, OpenGLMesh*>::iterator it = mesh_map.begin(); it != mesh_map.end(); it++) {
        delete it->second;
    }
    mesh_map.clear();
}

void OpenGLManager::cleanOpenGLTexture() {
    for (map<uint32_t, OpenGLTexture*>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
        delete it->second;
    }
    texture_map.clear();
}

// Convert generic Mesh to OpenGLMesh
OpenGLMesh * OpenGLManager::getOpenGLMesh(Mesh * mesh) {
    if (mesh_map.find(mesh->getID()) == mesh_map.end()) { // Not found
        OpenGLMesh* openGLMesh = new OpenGLMesh(mesh);
        vector<ShaderAttributeConfig> config = OpenGLConfig::getShaderAttributeConfig();
        for (uint32_t i = 0; i < config.size(); i++)
            openGLMesh->setVertexAttribPointer(config[i].indx, config[i].size, config[i].stride, config[i].offset);
        mesh_map[mesh->getID()] = openGLMesh;
    }
    return mesh_map[mesh->getID()];
}

// Convert generic Texture to OpenGLTexture
OpenGLTexture * OpenGLManager::getOpenGLTexture(Texture * texture) {
    if (texture_map.find(texture->getID()) == texture_map.end()) { // Not found
        OpenGLTexture* openGLTexture = new OpenGLTexture(texture);
        texture_map[texture->getID()] = openGLTexture;
    }
    return texture_map[texture->getID()];
}
