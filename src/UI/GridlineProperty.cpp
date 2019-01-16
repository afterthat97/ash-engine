#include <UI/GridlineProperty.h>

GridlineProperty::GridlineProperty(Gridline * gridline, QWidget * parent): QWidget(parent) {
    m_host = gridline;

    m_xargsEdit = new Vector3DEdit("X", Qt::Horizontal, "Start", "End", "Stride", -inf, inf, 2, this);
    m_yargsEdit = new Vector3DEdit("Y", Qt::Horizontal, "Start", "End", "Stride", -inf, inf, 2, this);
    m_zargsEdit = new Vector3DEdit("Z", Qt::Horizontal, "Start", "End", "Stride", -inf, inf, 2, this);
    m_colorEditSlider = new Vector3DEditSlider("Color", Qt::Horizontal, "R", "G", "B", 0.0f, 1.0f, 2, this);

    m_xargsEdit->setValue(QVector3D(m_host->xRange().first, m_host->xRange().second, m_host->xStride()));
    m_yargsEdit->setValue(QVector3D(m_host->yRange().first, m_host->yRange().second, m_host->yStride()));
    m_zargsEdit->setValue(QVector3D(m_host->zRange().first, m_host->zRange().second, m_host->zStride()));
    m_colorEditSlider->setValue(m_host->color());

    configLayout();
    configSignals();
}

void GridlineProperty::configLayout() {
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(m_xargsEdit);
    mainLayout->addWidget(m_yargsEdit);
    mainLayout->addWidget(m_zargsEdit);
    mainLayout->addWidget(m_colorEditSlider);
    setLayout(mainLayout);
}

void GridlineProperty::configSignals() {
    connect(m_xargsEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setXArguments(QVector3D)));
    connect(m_yargsEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setYArguments(QVector3D)));
    connect(m_zargsEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setZArguments(QVector3D)));
    connect(m_colorEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setColor(QVector3D)));

    connect(m_host, SIGNAL(xArgumentsChanged(QVector3D)), m_xargsEdit, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(yArgumentsChanged(QVector3D)), m_yargsEdit, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(zArgumentsChanged(QVector3D)), m_zargsEdit, SLOT(setValue(QVector3D)));
    connect(m_host, SIGNAL(colorChanged(QVector3D)), m_colorEditSlider, SLOT(setValue(QVector3D)));
}
