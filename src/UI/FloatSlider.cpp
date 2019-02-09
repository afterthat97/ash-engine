#include <FloatSlider.h>

FloatSlider::FloatSlider(QWidget *parent): QWidget(parent) {
    m_intSlider = new QSlider(Qt::Horizontal, this);
    m_intSlider->setMinimum(0);
    m_intSlider->setMaximum(10000);
    setRange(0.0f, 1.0f);
    setValue(0.0f);
    configLayout();
    configSignals();
}

FloatSlider::FloatSlider(Qt::Orientation orientation, float minimum, float maximum, QWidget * parent): QWidget(parent) {
    m_intSlider = new QSlider(orientation, this);
    m_intSlider->setMinimum(0);
    m_intSlider->setMaximum(10000);
    setRange(minimum, maximum);
    setValue(0.0f);
    configLayout();
    configSignals();
}

Qt::Orientation FloatSlider::orientation() const {
    return m_intSlider->orientation();
}

float FloatSlider::maximum() const {
    return m_maximum;
}

float FloatSlider::minimum() const {
    return m_minimum;
}

float FloatSlider::value() const {
    float v = float(m_intSlider->value()) / 10000.0f;
    return v * (m_maximum - m_minimum) + m_minimum;
}

void FloatSlider::setOrientation(Qt::Orientation orientation) {
    m_intSlider->setOrientation(orientation);
}

void FloatSlider::setRange(float minimum, float maximum) {
    m_minimum = minimum;
    m_maximum = maximum;
}

void FloatSlider::setValue(float value) {
    if (!qFuzzyCompare(this->value(), value)) {
        value = (value > m_maximum ? m_maximum : value);
        value = (value < m_minimum ? m_minimum : value);
        m_intSlider->setValue(int((value - m_minimum) / (m_maximum - m_minimum) * 10000.0f));
        valueChanged(this->value());
    }
}

void FloatSlider::configLayout() {
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(m_intSlider);
    setLayout(layout);
}

void FloatSlider::configSignals() {
    connect(m_intSlider, SIGNAL(sliderMoved(int)), this, SLOT(intSliderMoved(int)));
}

void FloatSlider::intSliderMoved(int) {
    sliderMoved(this->value());
    valueChanged(this->value());
}
