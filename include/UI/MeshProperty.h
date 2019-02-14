#pragma once

#include <Mesh.h>
#include <Vector3DEditSlider.h>

class MeshProperty: public QWidget {
    Q_OBJECT

public:
    MeshProperty(Mesh * mesh, QWidget * parent = 0);

private:
    Mesh *m_host;
    QCheckBox *m_visibleCheckBox, *m_wireFrameModeCheckBox;
    QLabel *m_meshTypeTextLabel, *m_meshTypeValueLabel;
    QLabel *m_numOfVerticesTextLabel, *m_numOfVerticesValueLabel;
    QLabel *m_numOfFacesTextLabel, *m_numOfFacesValueLabel;
    Vector3DEdit *m_positionEdit, *m_scalingEdit;
    Vector3DEditSlider *m_rotationEditSlider;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
    void meshTypeChanged(int meshType);
    void geometryChanged(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices);
};
