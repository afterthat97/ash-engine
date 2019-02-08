#pragma once

#include <UI/FloatEdit.h>

class Vector3DEdit: public QGroupBox {
    Q_OBJECT

public:
    Vector3DEdit(QWidget* parent = 0);
    Vector3DEdit(QString title,
                 Qt::Orientation orientation,
                 QString label0, QString label1, QString label2,
                 float minimum, float maximum, int decimals,
                 QWidget* parent = 0);

    QVector3D value();

public slots:
    void setLabelTexts(QString label0, QString label1, QString label2);
    void setRange(float minimum, float maximum);
    void setDecimals(int num);
    void setValue(QVector3D value);

signals:
    void valueEdited(QVector3D value);
    void valueChanged(QVector3D value);

protected:
    QLabel * m_labels[3];
    FloatEdit * m_floatEdits[3];

    virtual void configLayout(Qt::Orientation orientation);
    virtual void configSignals();

protected slots:
    void floatEditValueEdited(float value);
};
