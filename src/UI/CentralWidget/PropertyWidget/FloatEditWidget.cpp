#include <UI/CentralWidget/PropertyWidget/FloatEditWidget.h>

FloatEditWidget::FloatEditWidget(QString labelText, bool vertical, QWidget * parent): QWidget(parent) {
    label = new QLabel(labelText, this);
    spinBox = new QDoubleSpinBox(this);
    spinBox->setMinimumWidth(60);
    spinBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    setupLayout(vertical);
    setupSignals();
}

FloatEditWidget::~FloatEditWidget() {
    delete label;
    delete spinBox;
    delete mainLayout;
}

// Set properties

void FloatEditWidget::setLabelText(QString text) {
    label->setText(text);
}

void FloatEditWidget::setLabelWidget(int width) {
    label->setFixedWidth(width);
}

void FloatEditWidget::setRange(double minv, double maxv) {
    spinBox->setRange(minv, maxv);
}

void FloatEditWidget::setSingleStep(double step) {
    spinBox->setSingleStep(step);
}

void FloatEditWidget::setDecimals(int num) {
    spinBox->setDecimals(num);
}

// Get properties

float FloatEditWidget::getValue() {
    return spinBox->value();
}

// Public slots

void FloatEditWidget::setValue(float value) {
    spinBox->setValue(value);
}

// Private functions

void FloatEditWidget::setupLayout(bool vertical) {
    if (vertical) {
        mainLayout = new QVBoxLayout;
        mainLayout->setAlignment(Qt::AlignTop);
    } else {
        mainLayout = new QHBoxLayout;
        mainLayout->setAlignment(Qt::AlignLeft);
    }
    mainLayout->addWidget(label);
    mainLayout->addWidget(spinBox);
    setLayout(mainLayout);
}

void FloatEditWidget::setupSignals() {
    connect(spinBox, SIGNAL(editingFinished()), this, SLOT(spinBoxEditingFinished()));
}

// Private slots

void FloatEditWidget::spinBoxEditingFinished() {
    valueChanged(float(spinBox->value()));
}
