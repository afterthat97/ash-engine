#pragma once

#include <UI/Common.h>

class StringEditWidget: public QWidget {
    Q_OBJECT

public:
    StringEditWidget(QString labelText, QWidget* parent = 0);
    ~StringEditWidget();

    void setLabelText(QString text);
    QString getValue();

public slots:
    void setValue(QString value);

signals:
    void valueChanged(QString value);

private:
    QLabel * label;
    QLineEdit * lineEdit;
    QHBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();
};
