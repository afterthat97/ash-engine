#pragma once

#include <Core/Material.h>
#include <UI/Vector3DEditSlider.h>

class MaterialProperty: public QWidget {
    Q_OBJECT

public:
    MaterialProperty(Material* material, QWidget *parent = 0);

private:
    Material *m_host;
    QLabel *m_ambientLabel, *m_diffuseLabel, *m_specularLabel, *m_shininessLabel;
    FloatEdit *m_ambientEdit, *m_diffuseEdit, *m_specularEdit, *m_shininessEdit;
    FloatSlider *m_ambientSlider, *m_diffuseSlider, *m_specularSlider, *m_shininessSlider;
    Vector3DEditSlider *m_colorEditSlider;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject * host);
};
