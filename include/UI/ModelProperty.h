#pragma once

#include <Core/Model.h>
#include <UI/Vector3DEditSlider.h>

class ModelProperty: public QWidget {
    Q_OBJECT

public:
    ModelProperty(Model * model, QWidget * parent = 0);

private:
    Model *m_host;
    QCheckBox *m_visibleCheckBox;
    Vector3DEdit *m_positionEdit, *m_scalingEdit;
    Vector3DEditSlider *m_rotationEditSlider;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};
