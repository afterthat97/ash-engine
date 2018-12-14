#pragma once

#include <UI/Common.h>

class Vector3DWidget: public QGroupBox {
    Q_OBJECT

public:
    Vector3DWidget(QString title, bool showSlider = false, QWidget* parent = Q_NULLPTR);
    ~Vector3DWidget();

    void setLabelText(QString text0, QString text1, QString text2);
    void setRange(double minv, double maxv);
    void setSingleStep(double step);
    void setValue(QVector3D value);
    QVector3D getValue();

private:
    QLabel *label0, *label1, *label2;
    QDoubleSpinBox *spinBox0, *spinBox1, *spinBox2;
    QSlider *slider0, *slider1, *slider2;
    QVBoxLayout * mainLayout;
    QHBoxLayout * labelLayout, *spinBoxLayout, *sliderLayout;

    void setupLayout();
    void setupSignals();

signals:
    void valueChanged(QVector3D value);

private slots:
    void spinBox0ValueChanged(double value);
    void spinBox1ValueChanged(double value);
    void spinBox2ValueChanged(double value);
    void slider0ValueChanged(int value);
    void slider1ValueChanged(int value);
    void slider2ValueChanged(int value);
};