#pragma once

#include <UI/Common.h>

class FloatEditWidget: public QWidget {
    Q_OBJECT

public:
    FloatEditWidget(QString labelText, bool vertical, QWidget* parent = Q_NULLPTR);
    ~FloatEditWidget();

    void setLabelText(QString text);
    void setLabelWidget(int width);
    void setRange(double minv, double maxv);
    void setSingleStep(double step);
    void setDecimals(int num);
    float getValue();

public slots:
    void setValue(float value);

signals:
    void valueChanged(float value);

private:
    QLabel *label;
    QDoubleSpinBox *spinBox;
    QBoxLayout * mainLayout;

    void setupLayout(bool vertical);
    void setupSignals();

private slots:
    void spinBoxEditingFinished();
};