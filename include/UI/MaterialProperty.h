#pragma once

#include <UI/Common.h>
#include <UI/Vector3DEditSlider.h>

class MaterialProperty: public QWidget {
    Q_OBJECT

public:
    MaterialProperty(Material* material, QWidget *parent = 0);
    // TODO: Copy constructor

private:
    Material *m_host;
    QLabel *m_ambientLabel, *m_diffuseLabel, *m_specularLabel, *m_shininessLabel;
    FloatEdit *m_ambientEdit, *m_diffuseEdit, *m_specularEdit, *m_shininessEdit;
    FloatSlider *m_ambientSlider, *m_diffuseSlider, *m_specularSlider, *m_shininessSlider;
    Vector3DEditSlider *m_colorEditSlider;

    void configLayout();
    void configSignals();

private slots:
    // TODO: Connect to host's destroyed signal, commit suicide when host is destroyed.
};
