#include <UI/Vector3DWidget.h>

Vector3DWidget::Vector3DWidget(QString title, bool showSlider, QWidget * parent): QGroupBox(parent) {
    setTitle(title);
    label0 = new QLabel("Label0", this);
    label1 = new QLabel("Label1", this);
    label2 = new QLabel("Label2", this);
    spinBox0 = new QDoubleSpinBox(this);
    spinBox1 = new QDoubleSpinBox(this);
    spinBox2 = new QDoubleSpinBox(this);
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

Vector3DWidget::~Vector3DWidget() {
    delete label0;
    delete label1;
    delete label2;
    delete spinBox0;
    delete spinBox1;
    delete spinBox2;
    if (slider0) delete slider0;
    if (slider1) delete slider1;
    if (slider2) delete slider2;
}

void Vector3DWidget::setLabelText(QString text0, QString text1, QString text2) {
    label0->setText(text0);
    label1->setText(text1);
    label2->setText(text2);
}

void Vector3DWidget::setValue(QVector3D value) {
    spinBox0->setValue(value[0]);
    spinBox1->setValue(value[1]);
    spinBox2->setValue(value[2]);
    if (slider0) slider0->setValue(int(value[0] * 360));
    if (slider1) slider1->setValue(int(value[1] * 360));
    if (slider2) slider2->setValue(int(value[2] * 360));
}

void Vector3DWidget::setRange(double minv, double maxv) {
    spinBox0->setRange(minv, maxv);
    spinBox1->setRange(minv, maxv);
    spinBox2->setRange(minv, maxv);
}

void Vector3DWidget::setSingleStep(double step) {
    spinBox0->setSingleStep(step);
    spinBox1->setSingleStep(step);
    spinBox2->setSingleStep(step);
}

QVector3D Vector3DWidget::getValue() {
    return QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value());
}

void Vector3DWidget::setupLayout() {
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);

    QHBoxLayout * labelLayout = new QHBoxLayout;
    labelLayout->addWidget(label0);
    labelLayout->addWidget(label1);
    labelLayout->addWidget(label2);
    mainLayout->addLayout(labelLayout);

    QHBoxLayout * spinBoxLayout = new QHBoxLayout;
    spinBoxLayout->addWidget(spinBox0);
    spinBoxLayout->addWidget(spinBox1);
    spinBoxLayout->addWidget(spinBox2);
    mainLayout->addLayout(spinBoxLayout);

    if (slider0 && slider1 && slider2) {
        QHBoxLayout * sliderLayout = new QHBoxLayout;
        sliderLayout->addWidget(slider0);
        sliderLayout->addWidget(slider1);
        sliderLayout->addWidget(slider2);
        mainLayout->addLayout(sliderLayout);
    }
    setLayout(mainLayout);
}

void Vector3DWidget::setupSignals() {
    connect(spinBox0, SIGNAL(valueChanged(double)), this, SLOT(spinBox0ValueChanged(double)));
    connect(spinBox1, SIGNAL(valueChanged(double)), this, SLOT(spinBox1ValueChanged(double)));
    connect(spinBox2, SIGNAL(valueChanged(double)), this, SLOT(spinBox2ValueChanged(double)));
    if (slider0) connect(slider0, SIGNAL(valueChanged(int)), this, SLOT(slider0ValueChanged(int)));
    if (slider1) connect(slider1, SIGNAL(valueChanged(int)), this, SLOT(slider1ValueChanged(int)));
    if (slider2) connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(slider2ValueChanged(int)));
}

void Vector3DWidget::spinBox0ValueChanged(double value) {
    if (slider0) slider0->setValue(int(value * 360 / (spinBox0->maximum() - spinBox0->minimum())));
    valueChanged(QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value()));
}

void Vector3DWidget::spinBox1ValueChanged(double value) {
    if (slider1) slider1->setValue(int(value * 360 / (spinBox0->maximum() - spinBox0->minimum())));
    valueChanged(QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value()));
}

void Vector3DWidget::spinBox2ValueChanged(double value) {
    if (slider2) slider2->setValue(int(value * 360 / (spinBox0->maximum() - spinBox0->minimum())));
    valueChanged(QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value()));
}

void Vector3DWidget::slider0ValueChanged(int value) {
    spinBox0->setValue(value / 360.0 * (spinBox0->maximum() - spinBox0->minimum()));
    valueChanged(QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value()));
}

void Vector3DWidget::slider1ValueChanged(int value) {
    spinBox1->setValue(value / 360.0 * (spinBox1->maximum() - spinBox1->minimum()));
    valueChanged(QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value()));
}

void Vector3DWidget::slider2ValueChanged(int value) {
    spinBox2->setValue(value / 360.0 * (spinBox2->maximum() - spinBox2->minimum()));
    valueChanged(QVector3D(spinBox0->value(), spinBox1->value(), spinBox2->value()));
}
