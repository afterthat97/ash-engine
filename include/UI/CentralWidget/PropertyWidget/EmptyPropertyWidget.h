#pragma once

#include <UI/Common.h>

class EmptyPropertyWidget: public QWidget {
    Q_OBJECT

public:
    EmptyPropertyWidget(QWidget *parent = Q_NULLPTR);
    ~EmptyPropertyWidget();

private:
    QLabel * label;
    QVBoxLayout * mainLayout;
};