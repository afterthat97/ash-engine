#pragma once

#include <Core/Scene.h>
#include <OpenGL/Common.h>
#include <OpenGL/OpenGLMesh.h>
#include <OpenGL/OpenGLUniformBufferObject.h>

class OpenGLScene: public QObject {
    Q_OBJECT

public:
    OpenGLScene(Scene* scene);
    
    Scene* host() const;

    OpenGLMesh* pick(uint32_t pickingID);

    void renderAxis();
    void renderGridlines();
    void renderLights();
    void renderModels();

    void commitCameraInfo();
    void commitLightingInfo();

protected:
    void childEvent(QChildEvent *event) override;

private:
    Scene* m_host;
    QVector<OpenGLMesh*> m_axisMeshes, m_gridlineMeshes, m_lightMeshes, m_normalMeshes;
    static OpenGLUniformBufferObject *m_cameraInfo, *m_lightingInfo;

private slots:
    void axisAdded(Axis* axis);
    void gridlineAdded(Gridline* gridline);
    void lightAdded(AbstractLight* light);
    void modelAdded(Model* model);
    void hostDestroyed(QObject* host);
};
