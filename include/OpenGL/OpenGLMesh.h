#pragma once

#include <Core/Mesh.h>
#include <OpenGL/Common.h>
#include <OpenGL/OpenGLMaterial.h>

class OpenGLMesh: public QObject {
    Q_OBJECT

public:
    OpenGLMesh(Mesh* mesh, QObject* parent = 0);
    ~OpenGLMesh();

    Mesh* host() const;

    void create();
    void commitInfo();
    void render();
    void destroy();

public slots:
    void setSizeFixed(bool sizeFixed);
    void setPickingID(uint id);

protected:
    void childEvent(QChildEvent *event) override;

private:
    Mesh* m_host;
    bool m_sizeFixed;
    uint m_pickingID;
    
    QOpenGLVertexArrayObject * m_vao;
    QOpenGLBuffer * m_vbo, *m_ebo;
    QOpenGLFunctions_3_3_Core * glFuncs;
    OpenGLMaterial *m_openGLMaterial;

    static OpenGLUniformBufferObject *m_modelInfo;

private slots:
    void materialChanged(Material* material);
    void geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
    void hostDestroyed(QObject* host);
};
