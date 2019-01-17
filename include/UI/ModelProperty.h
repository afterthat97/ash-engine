#pragma once

#include <UI/Common.h>
#include <UI/Vector3DEditSlider.h>

class ModelProperty: public QWidget {
    Q_OBJECT

public:
    ModelProperty(Model * model, QWidget * parent = 0);
    // TODO: Copy constructor

private:
    Model *m_host;
    QCheckBox *m_visibleCheckBox;
    Vector3DEdit *m_positionEdit, *m_scalingEdit;
    Vector3DEditSlider *m_rotationEditSlider;

    void configLayout();
    void configSignals();

private slots:
    // TODO: Connect to host's destroyed signal, commit suicide when host is destroyed.
};
