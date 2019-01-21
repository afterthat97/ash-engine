#pragma once

#include <UI/Common.h>

class FloatEdit: public QWidget {
    Q_OBJECT

public:
    FloatEdit(QWidget* parent = 0);
    FloatEdit(float minimum, float maximum, int decimals,
              QWidget* parent = 0);

    float maximum() const;
    float minimum() const;
    int	decimals() const;
    float value() const;

public slots:
    void setRange(float minimum, float maximum);
    void setDecimals(int num);
    void setValue(float value);

signals:
    void valueEdited(float value);
    void valueChanged(float value);

private:
    float m_maximum, m_minimum, m_decimals;
    QLineEdit *m_lineEdit;

    void configLayout();
    void configSignals();

private slots:
    void editingFinished();
};
