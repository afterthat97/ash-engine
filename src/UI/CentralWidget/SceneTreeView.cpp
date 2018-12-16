#include <UI/CentralWidget/SceneTreeView.h>
#include <UI/MainWindow.h>

SceneTreeView::SceneTreeView(QWidget* parent): QTreeView(parent) {
    sceneTreeModel = new SceneTreeModel;
    setModel(sceneTreeModel);
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
}

SceneTreeView::~SceneTreeView() {
    delete sceneTreeModel;
}

void SceneTreeView::selectionChanged(const QModelIndex & cntIndex, const QModelIndex & prevIndex) {
    if (prevIndex.isValid()) {
        QObject * pointer = static_cast<QObject*>(prevIndex.internalPointer());
        if (Model* model = dynamic_cast<Model*>(pointer)) {
            modelSelected(model, false);
        } else if (Light* light = dynamic_cast<Light*>(pointer)) {
            lightSelected(light, false);
        } else if (Mesh* mesh = dynamic_cast<Mesh*>(pointer)) {
            meshSelected(mesh, false);
        } else if (Material* material = dynamic_cast<Material*>(pointer)) {
            materialSelected(material, false);
        } else if (Texture* texture = dynamic_cast<Texture*>(pointer)) {
            textureSelected(texture, false);
        }
    }
    if (cntIndex.isValid()) {
        QObject * pointer = static_cast<QObject*>(cntIndex.internalPointer());
        if (Model* model = dynamic_cast<Model*>(pointer)) {
            modelSelected(model, true);
        } else if (Light* light = dynamic_cast<Light*>(pointer)) {
            lightSelected(light, true);
        } else if (Mesh* mesh = dynamic_cast<Mesh*>(pointer)) {
            meshSelected(mesh, true);
        } else if (Material* material = dynamic_cast<Material*>(pointer)) {
            materialSelected(material, true);
        } else if (Texture* texture = dynamic_cast<Texture*>(pointer)) {
            textureSelected(texture, true);
        }
    }
}


SceneTreeModel::SceneTreeModel(QObject *parent): QAbstractItemModel(parent) {}

SceneTreeModel::~SceneTreeModel() {}

int SceneTreeModel::columnCount(const QModelIndex &) const {
    return 1;
}

QVariant SceneTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    QObject * pointer = static_cast<QObject*>(index.internalPointer());
    if (Model* item = dynamic_cast<Model*>(pointer)) {
        return QVariant("Model: " + item->objectName());
    } else if (Light* item = dynamic_cast<Light*>(pointer)) {
        return QVariant("Light: " + item->objectName());
    } else if (Mesh* item = dynamic_cast<Mesh*>(pointer)) {
        return QVariant("Mesh: " + item->objectName());
    } else if (Material* item = dynamic_cast<Material*>(pointer)) {
        return QVariant("Material: " + item->objectName());
    } else if (Texture* item = dynamic_cast<Texture*>(pointer)) {
        return QVariant("Texture: " + item->objectName());
    }
    return QVariant();
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return 0;
    return QAbstractItemModel::flags(index);
}

QVariant SceneTreeModel::headerData(int, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(Scene::currentScene()->objectName());
    return QVariant();
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parentModelIndex) const {
    if (!hasIndex(row, column, parentModelIndex)) return QModelIndex();

    QObject * parentPointer = static_cast<QObject*>(parentModelIndex.internalPointer());
    if (!parentModelIndex.isValid()) {
        Scene* parentItem = Scene::currentScene();
        if (row < parentItem->getLights().size())
            return createIndex(row, column, parentItem->getLights()[row]);
        else
            return createIndex(row, column, parentItem->getModels()[row - parentItem->getLights().size()]);
    } else if (Model* parentItem = dynamic_cast<Model*>(parentPointer)) {
        if (row < parentItem->getMeshes().size())
            return createIndex(row, column, parentItem->getMeshes()[row]);
        else
            return createIndex(row, column, parentItem->getChildren()[row - parentItem->getMeshes().size()]);
    } else if (Mesh* parentItem = dynamic_cast<Mesh*>(parentPointer)) {
        return createIndex(row, column, parentItem->getMaterial());
    } else if (Material* parentItem = dynamic_cast<Material*>(parentPointer)) {
        return createIndex(row, column, parentItem->getTextures()[row]);
    }
    return QModelIndex();
}

QModelIndex SceneTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();

    QObject * childPointer = static_cast<QObject*>(index.internalPointer())->parent();
    if (!childPointer || !childPointer->parent())
        return QModelIndex();

    QObject * parentPointer = childPointer->parent();
    if (Scene* parentItem = dynamic_cast<Scene*>(parentPointer)) {
        for (uint32_t i = 0; i < parentItem->getLights().size(); i++)
            if (parentItem->getLights()[i] == childPointer)
                return createIndex(i, 0, childPointer);
        for (uint32_t i = 0; i < parentItem->getModels().size(); i++)
            if (parentItem->getModels()[i] == childPointer)
                return createIndex(i + parentItem->getLights().size(), 0, childPointer);
    } else if (Model* parentItem = dynamic_cast<Model*>(parentPointer)) {
        for (uint32_t i = 0; i < parentItem->getMeshes().size(); i++)
            if (parentItem->getMeshes()[i] == childPointer)
                return createIndex(i, 0, childPointer);
        for (uint32_t i = 0; i < parentItem->getChildren().size(); i++)
            if (parentItem->getChildren()[i] == childPointer)
                return createIndex(i + parentItem->getMeshes().size(), 0, childPointer);
    } else if (Mesh* parentItem = dynamic_cast<Mesh*>(parentPointer)) {
        return createIndex(0, 0, childPointer);
    } else if (Material* parentItem = dynamic_cast<Material*>(parentPointer)) {
        for (uint32_t i = 0; i < parentItem->getTextures().size(); i++)
            if (parentItem->getTextures()[i] == childPointer)
                return createIndex(i, 0, childPointer);
    }
    return QModelIndex();
}

int SceneTreeModel::rowCount(const QModelIndex &parentModelIndex) const {
    QObject * parentPointer = static_cast<QObject*>(parentModelIndex.internalPointer());
    if (!parentModelIndex.isValid()) { // root
        Scene* parentItem = Scene::currentScene();
        return parentItem->getLights().size() + parentItem->getModels().size();
    } else if (Model* parentItem = dynamic_cast<Model*>(parentPointer)) {
        return parentItem->getChildren().size() + parentItem->getMeshes().size();
    } else if (Light* parentItem = dynamic_cast<Light*>(parentPointer)) {
        return 0;
    } else if (Mesh* parentItem = dynamic_cast<Mesh*>(parentPointer)) {
        return parentItem->getMaterial() != NULL;
    } else if (Material* parentItem = dynamic_cast<Material*>(parentPointer)) {
        return parentItem->getTextures().size();
    }
    return 0;
}
