#pragma once

#include <UI/Common.h>
#include <UI/Vector3DEditSlider.h>

class AmbientLightProperty: public QWidget {
    Q_OBJECT

public:
    AmbientLightProperty(AmbientLight * light, QWidget * parent = 0);
    // TODO: Copy constructor

private:
    AmbientLight* m_host;
    QCheckBox *m_enabledCheckBox;
    QLabel *m_intensityLabel;
    FloatEdit *m_intensityEdit;
    FloatSlider *m_intensitySlider;
    Vector3DEditSlider *m_colorEditSlider;

    void configLayout();
    void configSignals();

private slots:
    // TODO: Connect to host's destroyed signal, commit suicide when host is destroyed.
};
