#include <UI/SceneTreeView.h>

SceneTreeModel::SceneTreeModel(Scene* scene, QObject *parent): QAbstractItemModel(parent) {
    m_host = scene;
}

int SceneTreeModel::columnCount(const QModelIndex &) const {
    return 1;
}

// Return data for display
QVariant SceneTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    QObject * pointer = static_cast<QObject*>(index.internalPointer());
    if (Camera* camera = qobject_cast<Camera*>(pointer)) {
        return QVariant("Camera");
    } else if (Gridline* item = qobject_cast<Gridline*>(pointer)) {
        return QVariant("Gridline: " + item->objectName());
    } else if (AmbientLight* item = qobject_cast<AmbientLight*>(pointer)) {
        return QVariant("Ambient Light: " + item->objectName());
    } else if (DirectionalLight* item = qobject_cast<DirectionalLight*>(pointer)) {
        return QVariant("Directional Light: " + item->objectName());
    } else if (SpotLight* item = qobject_cast<SpotLight*>(pointer)) {
        return QVariant("Spot Light: " + item->objectName());
    } else if (PointLight* item = qobject_cast<PointLight*>(pointer)) {
        return QVariant("Point Light: " + item->objectName());
    } else if (Model* item = qobject_cast<Model*>(pointer)) {
        return QVariant("Model: " + item->objectName());
    } else if (Mesh* item = qobject_cast<Mesh*>(pointer)) {
        return QVariant("Mesh: " + item->objectName());
    } else if (Material* item = qobject_cast<Material*>(pointer)) {
        return QVariant("Material: " + item->objectName());
    }

    return QVariant();
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return 0;
    return QAbstractItemModel::flags(index);
}

// Return header data
QVariant SceneTreeModel::headerData(int, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_host->objectName());
    return QVariant();
}

// Get the row(th) child of parentModelIndex
QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parentModelIndex) const {
    if (!hasIndex(row, column, parentModelIndex)) return QModelIndex();

    QObject * parentPointer = static_cast<QObject*>(parentModelIndex.internalPointer());
    int idx = row;
    if (!parentModelIndex.isValid()) {
        if (idx == 0)
            return createIndex(row, column, m_host->camera());
        idx--;
        if (idx < m_host->gridlines().size())
            return createIndex(row, column, m_host->gridlines()[idx]);
        idx -= m_host->gridlines().size();
        if (idx < m_host->lights().size())
            return createIndex(row, column, m_host->lights()[idx]);
        idx -= m_host->lights().size();
        return createIndex(row, column, m_host->models()[idx]);
    } else if (Model* parentItem = qobject_cast<Model*>(parentPointer)) {
        if (idx < parentItem->childMeshes().size())
            return createIndex(row, column, parentItem->childMeshes()[idx]);
        idx -= parentItem->childMeshes().size();
        return createIndex(row, column, parentItem->childModels()[idx]);
    } else if (Mesh* parentItem = qobject_cast<Mesh*>(parentPointer)) {
        return createIndex(row, column, parentItem->material());
    }

    return QModelIndex();
}

// Return the ModelIndex of parent
QModelIndex SceneTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();

    QObject * parentPointer = static_cast<QObject*>(index.internalPointer())->parent();
    if (!parentPointer || parentPointer == m_host) return QModelIndex();

    QObject * grandParentPointer = parentPointer->parent();
    if (Scene* scene = qobject_cast<Scene*>(grandParentPointer)) {
        // Parent pointer must be Model*
        for (uint32_t i = 0; i < scene->models().size(); i++)
            if (scene->models()[i] == parentPointer)
                return createIndex(int(i + scene->lights().size() + 1), 0, parentPointer);
    } else if (Model* model = qobject_cast<Model*>(grandParentPointer)) {
        // Parent pointer could be Mesh* or Model*
        for (uint32_t i = 0; i < model->childMeshes().size(); i++)
            if (model->childMeshes()[i] == parentPointer)
                return createIndex(int(i), 0, parentPointer);
        for (uint32_t i = 0; i < model->childModels().size(); i++)
            if (model->childModels()[i] == parentPointer)
                return createIndex(int(i + model->childMeshes().size()), 0, parentPointer);
    } else if (Mesh* mesh = qobject_cast<Mesh*>(grandParentPointer)) {
        // Parent pointer must be Material*
        return createIndex(0, 0, parentPointer);
    }

    return QModelIndex();
}

// Return the number of children
int SceneTreeModel::rowCount(const QModelIndex &parentModelIndex) const {
    QObject * parentPointer = static_cast<QObject*>(parentModelIndex.internalPointer());
    if (!parentModelIndex.isValid()) { // Scene
        return 1 + m_host->gridlines().size() + m_host->lights().size() + m_host->models().size();
    } else if (Model* parentItem = qobject_cast<Model*>(parentPointer)) {
        return parentItem->childModels().size() + parentItem->childMeshes().size();
    } else if (Mesh* parentItem = qobject_cast<Mesh*>(parentPointer)) {
        return int(parentItem->material() != 0);
    }
    return 0;
}


SceneTreeView::SceneTreeView(Scene* scene, QWidget* parent): QTreeView(parent) {
    m_host = scene;
    setModel(new SceneTreeModel(m_host, this));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(selectionModelChanged(QModelIndex, QModelIndex)));
}

void SceneTreeView::selectionModelChanged(const QModelIndex& cntIndex, const QModelIndex& prevIndex) {
    if (prevIndex.isValid()) {
        QObject * pointer = static_cast<QObject*>(prevIndex.internalPointer());
        if (Camera* camera = qobject_cast<Camera*>(pointer)) {
            cameraDeselected(camera);
        } else if (Gridline* gridline = qobject_cast<Gridline*>(pointer)) {
            gridlineDeselected(gridline);
        } else if (Model* model = qobject_cast<Model*>(pointer)) {
            modelDeselected(model);
        } else if (AmbientLight* light = qobject_cast<AmbientLight*>(pointer)) {
            ambientLightDeselected(light);
        } else if (DirectionalLight* light = qobject_cast<DirectionalLight*>(pointer)) {
            directionalLightDeselected(light);
        } else if (SpotLight* light = qobject_cast<SpotLight*>(pointer)) {
            spotLightDeselected(light);
        } else if (PointLight* light = qobject_cast<PointLight*>(pointer)) {
            pointLightDeselected(light);
        } else if (Mesh* mesh = qobject_cast<Mesh*>(pointer)) {
            meshDeselected(mesh);
        } else if (Material* material = qobject_cast<Material*>(pointer)) {
            materialDeselected(material);
        }
    }
    if (cntIndex.isValid()) {
        QObject * pointer = static_cast<QObject*>(cntIndex.internalPointer());
        if (Camera* camera = qobject_cast<Camera*>(pointer)) {
            cameraSelected(camera);
        } else if (Gridline* gridline = qobject_cast<Gridline*>(pointer)) {
            gridlineSelected(gridline);
        } else if (Model* model = qobject_cast<Model*>(pointer)) {
            modelSelected(model);
        } else if (AmbientLight* light = qobject_cast<AmbientLight*>(pointer)) {
            ambientLightSelected(light);
        } else if (DirectionalLight* light = qobject_cast<DirectionalLight*>(pointer)) {
            directionalLightSelected(light);
        } else if (SpotLight* light = qobject_cast<SpotLight*>(pointer)) {
            spotLightSelected(light);
        } else if (PointLight* light = qobject_cast<PointLight*>(pointer)) {
            pointLightSelected(light);
        } else if (Mesh* mesh = qobject_cast<Mesh*>(pointer)) {
            meshSelected(mesh);
        } else if (Material* material = qobject_cast<Material*>(pointer)) {
            materialSelected(material);
        }
    }
}