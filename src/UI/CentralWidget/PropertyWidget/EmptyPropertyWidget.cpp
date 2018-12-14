#include <UI/CentralWidget/PropertyWidget/EmptyPropertyWidget.h>

EmptyPropertyWidget::EmptyPropertyWidget(QWidget * parent): QWidget(parent) {
    label = new QLabel("Empty property page", this);
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(label);
    setLayout(mainLayout);
}

EmptyPropertyWidget::~EmptyPropertyWidget() {
    delete label;
    delete mainLayout;
}
