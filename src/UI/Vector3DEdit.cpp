#include <UI/Vector3DEdit.h>

Vector3DEdit::Vector3DEdit(QWidget * parent): QGroupBox(parent) {
    for (int i = 0; i < 3; i++) {
        m_labels[i] = new QLabel(this);
        m_floatEdits[i] = new FloatEdit(this);
    }
    setTitle("Vector3D");
    setLabelTexts("label0", "label1", "label2");
    configLayout(Qt::Horizontal);
    configSignals();
}

Vector3DEdit::Vector3DEdit(QString title,
                           Qt::Orientation orientation,
                           QString label0, QString label1, QString label2,
                           float minimum, float maximum, int decimals,
                           QWidget * parent): QGroupBox(parent) {
    for (int i = 0; i < 3; i++) {
        m_labels[i] = new QLabel(this);
        m_floatEdits[i] = new FloatEdit(minimum, maximum, decimals, this);
    }
    setTitle(title);
    setLabelTexts(label0, label1, label2);
    configLayout(orientation);
    configSignals();
}

QVector3D Vector3DEdit::value() {
    return QVector3D(m_floatEdits[0]->value(), m_floatEdits[1]->value(), m_floatEdits[2]->value());
}

// Public slots

void Vector3DEdit::setLabelTexts(QString label0, QString label1, QString label2) {
    m_labels[0]->setText(label0);
    m_labels[1]->setText(label1);
    m_labels[2]->setText(label2);
}

void Vector3DEdit::setRange(float minimum, float maximum) {
    for (int i = 0; i < 3; i++)
        m_floatEdits[i]->setRange(minimum, maximum);
}

void Vector3DEdit::setDecimals(int num) {
    for (int i = 0; i < 3; i++)
        m_floatEdits[i]->setDecimals(num);
}

void Vector3DEdit::setValue(QVector3D value) {
    if (!qFuzzyCompare(this->value(), value)) {
        for (int i = 0; i < 3; i++)
            m_floatEdits[i]->setValue(value[i]);
        valueChanged(this->value());
    }
}

// Private functions

void Vector3DEdit::configLayout(Qt::Orientation orientation) {
    QGridLayout* mainLayout = new QGridLayout;
    for (int i = 0; i < 3; i++)
        if (orientation == Qt::Horizontal) {
            mainLayout->addWidget(m_labels[i], 0, i);
            mainLayout->addWidget(m_floatEdits[i], 1, i);
        } else {
            mainLayout->addWidget(m_labels[i], i, 0);
            mainLayout->addWidget(m_floatEdits[i], i, 1);
        }
    setLayout(mainLayout);
}

void Vector3DEdit::configSignals() {
    for (int i = 0; i < 3; i++)
        connect(m_floatEdits[i], SIGNAL(valueEdited(float)), this, SLOT(floatEditValueEdited(float)));
}

// Private slots

void Vector3DEdit::floatEditValueEdited(float) {
    valueEdited(this->value());
    valueChanged(this->value());
}