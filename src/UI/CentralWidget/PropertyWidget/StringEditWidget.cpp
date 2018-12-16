#include <UI/CentralWidget/PropertyWidget/StringEditWidget.h>

StringEditWidget::StringEditWidget(QString labelText, QWidget * parent): QWidget(parent) {
    label = new QLabel(labelText, this);
    lineEdit = new QLineEdit(this);

    setupLayout();
    setupSignals();
}

StringEditWidget::~StringEditWidget() {
    delete label;
    delete lineEdit;
    delete mainLayout;
}

// Set properties

void StringEditWidget::setLabelText(QString text) {
    label->setText(text);
}

// Get properties

QString StringEditWidget::getValue() {
    return lineEdit->text();
}

// Public slots

void StringEditWidget::setValue(QString value) {
    lineEdit->setText(value);
}

// Private functions

void StringEditWidget::setupLayout() {
    mainLayout = new QHBoxLayout;
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(label);
    mainLayout->addWidget(lineEdit);
    setLayout(mainLayout);
}

void StringEditWidget::setupSignals() {
    connect(lineEdit, SIGNAL(textEdited(QString)), this, SIGNAL(valueChanged(QString)));
}
