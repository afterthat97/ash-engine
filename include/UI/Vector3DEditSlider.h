#pragma once

#include <UI/Common.h>
#include <UI/FloatSlider.h>
#include <UI/Vector3DEdit.h>

class Vector3DEditSlider: public Vector3DEdit {
    Q_OBJECT

public:
    Vector3DEditSlider(QWidget* parent = 0);
    Vector3DEditSlider(QString title,
                       Qt::Orientation orientation,
                       QString label0, QString label1, QString label2,
                       float minimum, float maximum, int decimals,
                       QWidget* parent = 0);
    // TODO: Copy constructor

private:
    FloatSlider * m_floatSliders[3];

    void configLayout(Qt::Orientation orientation) override;
    void configSignals() override;

private slots:
    void floatSliderValueSlided(float value);
};
