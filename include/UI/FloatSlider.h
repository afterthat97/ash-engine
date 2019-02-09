#pragma once

#include <Common.h>

class FloatSlider: public QWidget {
    Q_OBJECT

public:
    FloatSlider(QWidget* parent = 0);
    FloatSlider(Qt::Orientation orientation,
                float minimum, float maximum,
                QWidget* parent = 0);

    Qt::Orientation	orientation() const;
    float maximum() const;
    float minimum() const;
    float value() const;

public slots:
    void setOrientation(Qt::Orientation orientation);
    void setRange(float minimum, float maximum);
    void setValue(float value);

signals:
    void sliderMoved(float value);
    void valueChanged(float value);

private:
    QSlider* m_intSlider;
    float m_minimum, m_maximum;

    void configLayout();
    void configSignals();

private slots:
    void intSliderMoved(int value);
};
