#include <OpenGL/OpenGLModel.h>
#include <OpenGL/OpenGLMesh.h>
#include <OpenGL/OpenGLManager.h>

OpenGLModel::OpenGLModel(Model * model) {
    m_host = model;
    connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

Model * OpenGLModel::host() const {
    return m_host;
}

void OpenGLModel::render(QOpenGLShaderProgram * shader) {
    if (!m_host->visible()) return;
    for (int i = 0; i < m_host->childMeshes().size(); i++) {
        Mesh* mesh = m_host->childMeshes()[i];
        OpenGLManager<Mesh, OpenGLMesh>::currentManager()->getOpenGLObject(mesh)->render(shader);
    }
    for (int i = 0; i < m_host->childModels().size(); i++) {
        Model* model = m_host->childModels()[i];
        OpenGLManager<Model, OpenGLModel>::currentManager()->getOpenGLObject(model)->render(shader);
    }
}

void OpenGLModel::hostDestroyed(QObject *) {
    // Remove entry
    OpenGLManager<Model, OpenGLModel>::currentManager()->removeOpenGLObject(m_host);

    // Commit suicide
    delete this;
}
