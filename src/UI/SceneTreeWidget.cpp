#include <UI/SceneTreeWidget.h>

SceneTreeWidget::SceneTreeWidget(Scene* scene, QWidget* parent): QTreeWidget(parent) {
    setScene(scene);
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
}

void SceneTreeWidget::setScene(Scene * scene) {
    m_host = scene;
    if (m_host) {
        connect(m_host, SIGNAL(childrenChanged()), this, SLOT(reload()));
        connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
        reload();
    }
}

void SceneTreeWidget::reload() {
    this->clear();
    this->setColumnCount(1);
    this->setHeaderLabel("Scene");
    this->setCurrentItem(0);
    if (!m_host) return;

    QTreeWidgetItem *rootItem = this->invisibleRootItem();

    QTreeWidgetItem *cameraItem = new QTreeWidgetItem(rootItem);
    cameraItem->setText(0, m_host->camera()->objectName());
    cameraItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->camera()));
    cameraItem->setIcon(0, QIcon(":/resources/icons/CameraIcon.png"));

    for (int i = 0; i < m_host->gridlines().size(); i++) {
        QTreeWidgetItem *gridlineItem = new QTreeWidgetItem(rootItem);
        gridlineItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->gridlines()[i]));
        gridlineItem->setText(0, m_host->gridlines()[i]->objectName());
        gridlineItem->setIcon(0, QIcon(":/resources/icons/GridlineIcon.png"));
    }

    for (int i = 0; i < m_host->ambientLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->ambientLights()[i]));
        lightItem->setText(0, m_host->ambientLights()[i]->objectName());
        lightItem->setIcon(0, QIcon(":/resources/icons/AmbientLightIcon.png"));
    }

    for (int i = 0; i < m_host->directionalLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->directionalLights()[i]));
        lightItem->setText(0, m_host->directionalLights()[i]->objectName());
        lightItem->setIcon(0, QIcon(":/resources/icons/DirectionalLightIcon.png"));
    }

    for (int i = 0; i < m_host->pointLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->pointLights()[i]));
        lightItem->setText(0, m_host->pointLights()[i]->objectName());
        lightItem->setIcon(0, QIcon(":/resources/icons/PointLightIcon.png"));
    }

    for (int i = 0; i < m_host->spotLights().size(); i++) {
        QTreeWidgetItem *lightItem = new QTreeWidgetItem(rootItem);
        lightItem->setData(0, Qt::UserRole, QVariant::fromValue(m_host->spotLights()[i]));
        lightItem->setText(0, m_host->spotLights()[i]->objectName());
        lightItem->setIcon(0, QIcon(":/resources/icons/SpotLightIcon.png"));
    }

    for (int i = 0; i < m_host->models().size(); i++)
        createModelItem(rootItem, m_host->models()[i]);
}

void SceneTreeWidget::keyPressEvent(QKeyEvent * e) {
    if (!currentItem()) return;
    if (e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) {
        QVariant item = currentItem()->data(0, Qt::UserRole);
        itemDeselected(item);
        if (item.canConvert<Camera*>()) {
            delete item.value<Camera*>();
        } else if (item.canConvert<Gridline*>()) {
            delete item.value<Gridline*>();
        } else if (item.canConvert<AbstractLight*>()) {
            delete item.value<AbstractLight*>();
        } else if (item.canConvert<Model*>()) {
            delete item.value<Model*>();
        } else if (item.canConvert<Mesh*>()) {
            delete item.value<Mesh*>();
        } else if (item.canConvert<Material*>()) {
            delete item.value<Material*>();
        }
    }
}

QTreeWidgetItem * SceneTreeWidget::createModelItem(QTreeWidgetItem * parent, Model * model) {
    QTreeWidgetItem * modelItem = new QTreeWidgetItem(parent);
    modelItem->setData(0, Qt::UserRole, QVariant::fromValue(model));
    modelItem->setText(0, model->objectName());
    modelItem->setIcon(0, QIcon(":/resources/icons/ModelIcon.png"));
    for (int i = 0; i < model->childMeshes().size(); i++) {
        QTreeWidgetItem * meshItem = new QTreeWidgetItem(modelItem);
        meshItem->setData(0, Qt::UserRole, QVariant::fromValue(model->childMeshes()[i]));
        meshItem->setText(0, model->childMeshes()[i]->objectName());
        meshItem->setIcon(0, QIcon(":/resources/icons/MeshIcon.png"));
        if (model->childMeshes()[i]->material()) {
            QTreeWidgetItem * materialItem = new QTreeWidgetItem(meshItem);
            materialItem->setData(0, Qt::UserRole, QVariant::fromValue(model->childMeshes()[i]->material()));
            materialItem->setText(0, model->childMeshes()[i]->material()->objectName());
            materialItem->setIcon(0, QIcon(":/resources/icons/MaterialIcon.png"));
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

void SceneTreeWidget::hostDestroyed(QObject * host) {
    if (host == m_host) {
        m_host = 0;
        reload();
    }
}
