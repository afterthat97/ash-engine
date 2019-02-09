#pragma once

#include <PointLight.h>
#include <Vector3DEditSlider.h>

class PointLightProperty: public QWidget {
    Q_OBJECT

public:
    PointLightProperty(PointLight * light, QWidget * parent = 0);

private:
    PointLight *m_host;
    QCheckBox *m_enabledCheckBox;
    QLabel *m_intensityLabel;
    FloatEdit *m_intensityEdit;
    FloatSlider *m_intensitySlider;
    Vector3DEditSlider *m_colorEditSlider;
    Vector3DEdit *m_positionEdit, *m_attenuationEdit;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};
