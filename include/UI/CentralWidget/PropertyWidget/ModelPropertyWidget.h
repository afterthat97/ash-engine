#pragma once

#include <Generic/Model.h>
#include <UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h>

class ModelPropertyWidget: public QWidget {
    Q_OBJECT

public:
    ModelPropertyWidget(Model* model, QWidget *parent = 0);
    ~ModelPropertyWidget();

private:
    Model * model;
    QCheckBox * modelVisibleCheckbox;
    Vector3DEditWidget * modelPositionWidget, *modelRotationWidget, *modelScalingWidget;
    QPushButton * resetTransformationButton, *resetChildrenTransformationButton;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void setModelVisible(int state);
    void resetTransformationButtonClicked();
    void resetChildrenTransformationButtonClicked();
};
