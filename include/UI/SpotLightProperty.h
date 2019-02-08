#pragma once

#include <Core/Light/SpotLight.h>
#include <UI/Vector3DEditSlider.h>

class SpotLightProperty: public QWidget {
    Q_OBJECT

public:
    SpotLightProperty(SpotLight * light, QWidget * parent = 0);

private:
    SpotLight *m_host;
    QCheckBox *m_enabledCheckBox;
    QLabel *m_intensityLabel, *m_innerCutOffLabel, *m_outerCutOffLabel;
    FloatEdit *m_intensityEdit, *m_innerCutOffEdit, *m_outerCutOffEdit;
    FloatSlider *m_intensitySlider, *m_innerCutOffSlider, *m_outerCutOffSlider;
    Vector3DEditSlider *m_colorEditSlider;
    Vector3DEdit *m_positionEdit, *m_directionEdit, *m_attenuationEdit;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};
