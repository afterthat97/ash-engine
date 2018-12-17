#pragma once

#include <UI/Common.h>
#include <UI/CentralWidget/PropertyWidget/FloatEditWidget.h>

class Vector3DEditWidget: public QGroupBox {
    Q_OBJECT

public:
    Vector3DEditWidget(QString title, bool showSlider = false, QWidget* parent = Q_NULLPTR);
    ~Vector3DEditWidget();

    void setLabelText(QString text0, QString text1, QString text2);
    void setRange(double minv, double maxv);
    void setSingleStep(double step);
    void setDecimals(int num);
    QVector3D getValue();

public slots:
    void setValue(QVector3D value);

signals:
    void valueChanged(QVector3D value);

private:
    float range, minv;
    FloatEditWidget * floatEdit0, *floatEdit1, *floatEdit2;
    QSlider *slider0, *slider1, *slider2;
    QVBoxLayout * mainLayout;
    QHBoxLayout * floatEditLayout, *sliderLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void floatEdit0ValueChanged(float value);
    void floatEdit1ValueChanged(float value);
    void floatEdit2ValueChanged(float value);
    void slider0ValueChanged(int value);
    void slider1ValueChanged(int value);
    void slider2ValueChanged(int value);
};