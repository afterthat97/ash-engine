#pragma once

#include <Model.h>
#include <Vector3DEditSlider.h>

class ModelProperty: public QWidget {
    Q_OBJECT

public:
    ModelProperty(Model * model, QWidget * parent = 0);

private:
    Model *m_host;
    QCheckBox *m_visibleCheckBox;
    QLabel *m_numOfChildMeshesTextLabel, *m_numOfChildMeshesValueLabel;
    QLabel *m_numOfChildModelsTextLabel, *m_numOfChildModelsValueLabel;
    Vector3DEdit *m_positionEdit, *m_scalingEdit;
    Vector3DEditSlider *m_rotationEditSlider;
    QPushButton *m_resetTransformationButton;
    QPushButton *m_resetChildrenTransformationButton;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
    void childMeshAdded(Mesh* mesh);
    void childMeshRemoved(QObject* object);
    void childModelAdded(Model* model);
    void childModelRemoved(QObject* object);
};
