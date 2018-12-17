#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>

Vector3DEditWidget::Vector3DEditWidget(QString title, bool showSlider, QWidget * parent): QGroupBox(parent) {
    minv = 0.0;
    range = 1.0;
    setTitle(title);

    floatEdit0 = new FloatEditWidget("label0", true, this);
    floatEdit1 = new FloatEditWidget("label1", true, this);
    floatEdit2 = new FloatEditWidget("label2", true, this);

    if (showSlider) {
        slider0 = new QSlider(Qt::Horizontal, this);
        slider1 = new QSlider(Qt::Horizontal, this);
        slider2 = new QSlider(Qt::Horizontal, this);
        slider0->setRange(0, 360);
        slider1->setRange(0, 360);
        slider2->setRange(0, 360);
    } else
        slider0 = slider1 = slider2 = NULL;

    setupLayout();
    setupSignals();
}

Vector3DEditWidget::~Vector3DEditWidget() {
    delete floatEdit0;
    delete floatEdit1;
    delete floatEdit2;
    if (slider0) delete slider0;
    if (slider1) delete slider1;
    if (slider2) delete slider2;
    delete floatEditLayout;
    if (sliderLayout) delete sliderLayout;
    delete mainLayout;
}

// Set properties

void Vector3DEditWidget::setLabelText(QString text0, QString text1, QString text2) {
    floatEdit0->setLabelText(text0);
    floatEdit1->setLabelText(text1);
    floatEdit2->setLabelText(text2);
}

void Vector3DEditWidget::setRange(double _minv, double _maxv) {
    minv = _minv;
    range = _maxv - _minv;
    if (range < 0) {
        QMessageBox::critical(0, "Error", "Invalid range setting for Vector3DWidget");
        return;
    }
    floatEdit0->setRange(_minv, _maxv);
    floatEdit1->setRange(_minv, _maxv);
    floatEdit2->setRange(_minv, _maxv);
}

void Vector3DEditWidget::setSingleStep(double step) {
    floatEdit0->setSingleStep(step);
    floatEdit1->setSingleStep(step);
    floatEdit2->setSingleStep(step);
}

void Vector3DEditWidget::setDecimals(int num) {
    floatEdit0->setDecimals(num);
    floatEdit1->setDecimals(num);
    floatEdit2->setDecimals(num);
}

// Get properties

QVector3D Vector3DEditWidget::getValue() {
    return QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue());
}

// Public slots

void Vector3DEditWidget::setValue(QVector3D value) {
    floatEdit0->setValue(value[0]);
    floatEdit1->setValue(value[1]);
    floatEdit2->setValue(value[2]);
    if (slider0) slider0->setValue(int((value[0] - minv) / range * 360.0));
    if (slider1) slider1->setValue(int((value[1] - minv) / range * 360.0));
    if (slider2) slider2->setValue(int((value[2] - minv) / range * 360.0));
}

// Private functions

void Vector3DEditWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);

    floatEditLayout = new QHBoxLayout;
    floatEditLayout->addWidget(floatEdit0);
    floatEditLayout->addWidget(floatEdit1);
    floatEditLayout->addWidget(floatEdit2);
    mainLayout->addLayout(floatEditLayout);

    if (slider0 && slider1 && slider2) {
        sliderLayout = new QHBoxLayout;
        sliderLayout->addWidget(slider0);
        sliderLayout->addWidget(slider1);
        sliderLayout->addWidget(slider2);
        mainLayout->addLayout(sliderLayout);
    } else
        sliderLayout = NULL;
    setLayout(mainLayout);
}

void Vector3DEditWidget::setupSignals() {
    connect(floatEdit0, SIGNAL(valueChanged(float)), this, SLOT(floatEdit0ValueChanged(float)));
    connect(floatEdit1, SIGNAL(valueChanged(float)), this, SLOT(floatEdit1ValueChanged(float)));
    connect(floatEdit2, SIGNAL(valueChanged(float)), this, SLOT(floatEdit2ValueChanged(float)));
    if (slider0) connect(slider0, SIGNAL(sliderMoved(int)), this, SLOT(slider0ValueChanged(int)));
    if (slider1) connect(slider1, SIGNAL(sliderMoved(int)), this, SLOT(slider1ValueChanged(int)));
    if (slider2) connect(slider2, SIGNAL(sliderMoved(int)), this, SLOT(slider2ValueChanged(int)));
}

// Private slots

void Vector3DEditWidget::floatEdit0ValueChanged(float value) {
    if (slider0) slider0->setValue(int((value - minv) / range * 360.0));
    valueChanged(QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue()));
}

void Vector3DEditWidget::floatEdit1ValueChanged(float value) {
    if (slider1) slider1->setValue(int((value - minv) / range * 360.0));
    valueChanged(QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue()));
}

void Vector3DEditWidget::floatEdit2ValueChanged(float value) {
    if (slider2) slider2->setValue(int((value - minv) / range * 360.0));
    valueChanged(QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue()));
}

void Vector3DEditWidget::slider0ValueChanged(int value) {
    floatEdit0->setValue(value / 360.0 * range + minv);
    valueChanged(QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue()));
}

void Vector3DEditWidget::slider1ValueChanged(int value) {
    floatEdit1->setValue(value / 360.0 * range + minv);
    valueChanged(QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue()));
}

void Vector3DEditWidget::slider2ValueChanged(int value) {
    floatEdit2->setValue(value / 360.0 * range + minv);
    valueChanged(QVector3D(floatEdit0->getValue(), floatEdit1->getValue(), floatEdit2->getValue()));
}
