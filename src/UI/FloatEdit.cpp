#include <FloatEdit.h>

FloatEdit::FloatEdit(QWidget * parent): QWidget(parent) {
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setText(QString::number(0.0, 'f', 2));
    setRange(0.0f, 1.0f);
    setDecimals(2);
    setValue(0.0f);
    configLayout();
    configSignals();
}

FloatEdit::FloatEdit(float minimum, float maximum, int decimals,
                     QWidget * parent): QWidget(parent) {
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setText(QString::number(0.0, 'f', decimals));
    setRange(minimum, maximum);
    setDecimals(decimals);
    setValue(0.0f);
    configLayout();
    configSignals();
}

float FloatEdit::maximum() const {
    return m_maximum;
}

float FloatEdit::minimum() const {
    return m_minimum;
}

int FloatEdit::decimals() const {
    return m_decimals;
}

float FloatEdit::value() const {
    return m_lineEdit->text().toFloat();
}

void FloatEdit::setRange(float minimum, float maximum) {
    m_minimum = minimum;
    m_maximum = maximum;
}

void FloatEdit::setDecimals(int num) {
    m_decimals = num;
}

void FloatEdit::setValue(float value) {
    if (!isEqual(this->value(), value)) {
        m_lineEdit->setText(QString::number((double) value, 'f', m_decimals));
        valueChanged(value);
    }
}

void FloatEdit::configLayout() {
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_lineEdit);
    setLayout(mainLayout);
}

void FloatEdit::configSignals() {
    connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
}

void FloatEdit::editingFinished() {
    bool isFloat;
    float v = m_lineEdit->text().toFloat(&isFloat);

    if (isFloat) {
        v = (v > (float) m_maximum ? m_maximum : v);
        v = (v < (float) m_minimum ? m_minimum : v);
        m_lineEdit->setText(QString::number((double) v, 'f', m_decimals));
        valueEdited(v);
        valueChanged(v);
    } else {
        m_lineEdit->undo();
    }
}
