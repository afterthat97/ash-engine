#include <UI/SceneTreeWidget.h>

SceneTreeWidget::SceneTreeWidget(Scene* scene, QWidget* parent): QTreeWidget(parent) {
    setScene(scene);
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
}

void SceneTreeWidget::setScene(Scene * scene) {
    m_host = scene;
    if (m_host) {
        connect(m_host, SIGNAL(sceneChanged()), this, SLOT(hostChanged()));
        connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
        reload();
    }
}

void SceneTreeWidget::reload() {
    while (this->topLevelItemCount())
        delete this->takeTopLevelItem(0);
    this->setColumnCount(1);
    this->setHeaderLabel("Scene");

    QTreeWidgetItem *rootItem = this->invisibleRootItem();

    QTreeWidgetItem *cameraItem = new QTreeWidgetItem(rootItem);
    cameraItem->setText(0, "Camera");
    cameraItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->camera()));
    
    for (int i = 0; i < m_host->gridlines().size(); i++) {
        QTreeWidgetItem *gridlineItem = new QTreeWidgetItem(rootItem);
        gridlineItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->gridlines()[i]));
        gridlineItem->setText(0, "Gridline" + QString::number(i));
    }

    for (int i = 0; i < m_host->ambientLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->ambientLights()[i]));
        lightItem->setText(0, "Ambient Light" + QString::number(i));
    }

    for (int i = 0; i < m_host->directionalLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->directionalLights()[i]));
        lightItem->setText(0, "Directional Light" + QString::number(i));
    }

    for (int i = 0; i < m_host->pointLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->pointLights()[i]));
        lightItem->setText(0, "Point Light" + QString::number(i));
    }

    for (int i = 0; i < m_host->spotLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->spotLights()[i]));
        lightItem->setText(0, "Spot Light" + QString::number(i));
    }

    for (int i = 0; i < m_host->models().size(); i++)
        createModelItem(rootItem, m_host->models()[i]);
}

QTreeWidgetItem * SceneTreeWidget::createModelItem(QTreeWidgetItem * parent, Model * model) {
    QTreeWidgetItem * modelItem = new QTreeWidgetItem(parent);
    modelItem->setData(0, Qt::UserRole, QVariant::fromValue(model));
    modelItem->setText(0, model->objectName());
    for (int i = 0; i < model->childMeshes().size(); i++) {
        QTreeWidgetItem * meshItem = new QTreeWidgetItem(modelItem);
        meshItem->setData(0, Qt::UserRole, QVariant::fromValue(model->childMeshes()[i]));
        meshItem->setText(0, model->childMeshes()[i]->objectName());
        if (model->childMeshes()[i]->material()) {
            QTreeWidgetItem * materialItem = new QTreeWidgetItem(meshItem);
            materialItem->setData(0, Qt::UserRole, QVariant::fromValue(model->childMeshes()[i]->material()));
            materialItem->setText(0, model->childMeshes()[i]->material()->objectName());
        }
        modelItem->addChild(meshItem);
    }
    for (int i = 0; i < model->childModels().size(); i++) {
        QTreeWidgetItem * childModelItem = createModelItem(modelItem, model->childModels()[i]);
        modelItem->addChild(childModelItem);
    }
    return modelItem;
}

void SceneTreeWidget::currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous) {
    if (previous)
        itemDeselected(previous->data(0, Qt::UserRole));
    if (current)
        itemSelected(current->data(0, Qt::UserRole));
}

void SceneTreeWidget::hostChanged() {
    reload();
}

void SceneTreeWidget::hostDestroyed(QObject * host) {
    if (host == m_host) {
        // TODO
        m_host = 0;
    }
}
