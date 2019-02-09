#include <SceneTreeWidget.h>

SceneTreeWidget::SceneTreeWidget(Scene* scene, QWidget* parent): QTreeWidget(parent) {
    setScene(scene);
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
}

void SceneTreeWidget::setScene(Scene * scene) {
    m_host = scene;
    if (m_host) {
        connect(m_host, SIGNAL(gridlineAdded(Gridline*)), this, SLOT(gridlineAdded(Gridline*)));
        connect(m_host, SIGNAL(lightAdded(AbstractLight*)), this, SLOT(lightAdded(AbstractLight*)));
        connect(m_host, SIGNAL(modelAdded(Model*)), this, SLOT(modelAdded(Model*)));
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

    new CameraItem(m_host->camera(), invisibleRootItem());
    for (int i = 0; i < m_host->gridlines().size(); i++)
        new GridlineItem(m_host->gridlines()[i], invisibleRootItem());
    for (int i = 0; i < m_host->ambientLights().size(); i++)
        new AmbientLightItem(m_host->ambientLights()[i], invisibleRootItem());
    for (int i = 0; i < m_host->directionalLights().size(); i++)
        new DirectionalLightItem(m_host->directionalLights()[i], invisibleRootItem());
    for (int i = 0; i < m_host->pointLights().size(); i++)
        new PointLightItem(m_host->pointLights()[i], invisibleRootItem());
    for (int i = 0; i < m_host->spotLights().size(); i++)
        new SpotLightItem(m_host->spotLights()[i], invisibleRootItem());
    for (int i = 0; i < m_host->models().size(); i++)
        new ModelItem(m_host->models()[i], invisibleRootItem());
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

void SceneTreeWidget::currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous) {
    if (previous)
        itemDeselected(previous->data(0, Qt::UserRole));
    if (current)
        itemSelected(current->data(0, Qt::UserRole));
}

void SceneTreeWidget::gridlineAdded(Gridline * gridline) {
    for (int i = 0; i < topLevelItemCount(); i++)
        if (static_cast<BaseItem*>(topLevelItem(i))->priority() < GRIDLINE_PRIORITY) {
            insertTopLevelItem(i, new GridlineItem(gridline, 0));
            return;
        }
    addTopLevelItem(new GridlineItem(gridline, 0));
}

void SceneTreeWidget::lightAdded(AbstractLight * l) {
    if (SpotLight* light = qobject_cast<SpotLight*>(l)) {
        for (int i = 0; i < topLevelItemCount(); i++)
            if (static_cast<BaseItem*>(topLevelItem(i))->priority() < SPOTLIGHT_PRIORITY) {
                insertTopLevelItem(i, new SpotLightItem(light, 0));
                return;
            }
        addTopLevelItem(new SpotLightItem(light, 0));
    } else if (AmbientLight* light = qobject_cast<AmbientLight*>(l)) {
        for (int i = 0; i < topLevelItemCount(); i++)
            if (static_cast<BaseItem*>(topLevelItem(i))->priority() < AMBIENTLIGHT_PRIORITY) {
                insertTopLevelItem(i, new AmbientLightItem(light, 0));
                return;
            }
        addTopLevelItem(new AmbientLightItem(light, 0));
    } else if (DirectionalLight* light = qobject_cast<DirectionalLight*>(l)) {
        for (int i = 0; i < topLevelItemCount(); i++)
            if (static_cast<BaseItem*>(topLevelItem(i))->priority() < DIRECTIONALLIGHT_PRIORITY) {
                insertTopLevelItem(i, new DirectionalLightItem(light, 0));
                return;
            }
        addTopLevelItem(new DirectionalLightItem(light, 0));
    } else if (PointLight* light = qobject_cast<PointLight*>(l)) {
        for (int i = 0; i < topLevelItemCount(); i++)
            if (static_cast<BaseItem*>(topLevelItem(i))->priority() < POINTLIGHT_PRIORITY) {
                insertTopLevelItem(i, new PointLightItem(light, 0));
                return;
            }
        addTopLevelItem(new PointLightItem(light, 0));
    }
}

void SceneTreeWidget::modelAdded(Model * model) {
    addTopLevelItem(new ModelItem(model, 0));
}

void SceneTreeWidget::hostDestroyed(QObject * host) {
    if (host == m_host) {
        m_host = 0;
        reload();
    }
}
