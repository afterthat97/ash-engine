#include <UI/CentralWidget/PropertyWidget/MaterialPropertyWidget.h>

MaterialPropertyWidget::MaterialPropertyWidget(Material* _material, QWidget *parent): QWidget(parent) {
    material = _material;

    ambientColorWidget = new Vector3DEditWidget("Ambient Color", true, this);
    ambientColorWidget->setLabelText("R", "G", "B");
    ambientColorWidget->setRange(0.0, 1.0);
    ambientColorWidget->setSingleStep(0.01);
    ambientColorWidget->setValue(material->getAmbientColor());

    diffuseColorWidget = new Vector3DEditWidget("Diffuse Color", true, this);
    diffuseColorWidget->setLabelText("R", "G", "B");
    diffuseColorWidget->setRange(0.0, 1.0);
    diffuseColorWidget->setSingleStep(0.01);
    diffuseColorWidget->setValue(material->getDiffuseColor());

    specularColorWidget = new Vector3DEditWidget("Specular Color", true, this);
    specularColorWidget->setLabelText("R", "G", "B");
    specularColorWidget->setRange(0.0, 1.0);
    specularColorWidget->setSingleStep(0.01);
    specularColorWidget->setValue(material->getSpecularColor());

    shininessEditWidget = new FloatEditWidget("Shininess: ", false, this);
    shininessEditWidget->setRange(0.0, 1000000.0);
    shininessEditWidget->setSingleStep(1.0);
    shininessEditWidget->setValue(material->getShininess());

    setupLayout();
    setupSignals();
}

MaterialPropertyWidget::~MaterialPropertyWidget() {
    delete ambientColorWidget;
    delete diffuseColorWidget;
    delete specularColorWidget;
    delete shininessEditWidget;
    delete mainLayout;
}

void MaterialPropertyWidget::setupLayout() {
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(ambientColorWidget);
    mainLayout->addWidget(diffuseColorWidget);
    mainLayout->addWidget(specularColorWidget);
    mainLayout->addWidget(shininessEditWidget);
    setLayout(mainLayout);
}

void MaterialPropertyWidget::setupSignals() {
    connect(ambientColorWidget, SIGNAL(valueChanged(QVector3D)), material, SLOT(setAmbientColor(QVector3D)));
    connect(diffuseColorWidget, SIGNAL(valueChanged(QVector3D)), material, SLOT(setDiffuseColor(QVector3D)));
    connect(specularColorWidget, SIGNAL(valueChanged(QVector3D)), material, SLOT(setSpecularColor(QVector3D)));
    connect(shininessEditWidget, SIGNAL(valueChanged(float)), material, SLOT(setShininess(float)));
}
