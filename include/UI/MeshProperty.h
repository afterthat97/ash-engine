#pragma once

#include <Mesh.h>
#include <Vector3DEditSlider.h>

class MeshProperty: public QWidget {
    Q_OBJECT

public:
    MeshProperty(Mesh * mesh, QWidget * parent = 0);

private:
    Mesh *m_host;
    QCheckBox *m_visibleCheckBox;
    Vector3DEdit *m_positionEdit, *m_scalingEdit;
    Vector3DEditSlider *m_rotationEditSlider;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};
