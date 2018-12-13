#include <UI/SceneTreeView.h>

SceneTreeView::SceneTreeView(QWidget* parent): QTreeView(parent) {}

SceneTreeView::~SceneTreeView() {}

SceneTreeModel::SceneTreeModel(QObject *parent): QAbstractItemModel(parent) {}

SceneTreeModel::~SceneTreeModel() {}

int SceneTreeModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant SceneTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    Object * pointer = static_cast<Object*>(index.internalPointer());
    if (Model* item = dynamic_cast<Model*>(pointer)) {
        return QVariant(QStringLiteral("模型: ") + item->getName());
    } else if (Light* item = dynamic_cast<Light*>(pointer)) {
        return QVariant(QStringLiteral("光源: ") + item->getName());
    } else if (Mesh* item = dynamic_cast<Mesh*>(pointer)) {
        return QVariant(QStringLiteral("网格: ") + item->getName());
    } else if (Material* item = dynamic_cast<Material*>(pointer)) {
        return QVariant(QStringLiteral("材质: ") + item->getName());
    } else if (Texture* item = dynamic_cast<Texture*>(pointer)) {
        return QVariant(QStringLiteral("贴图: ") + item->getName());
    }
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return 0;
    return QAbstractItemModel::flags(index);
}

QVariant SceneTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(Scene::currentScene()->getName());
    return QVariant();
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parentModelIndex) const {
    if (!hasIndex(row, column, parentModelIndex)) return QModelIndex();

    Object * parentPointer = static_cast<Object*>(parentModelIndex.internalPointer());
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

    Object * childPointer = static_cast<Object*>(index.internalPointer())->getParent();
    if (!childPointer || !childPointer->getParent())
        return QModelIndex();

    Object * parentPointer = childPointer->getParent();
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
    Object * parentPointer = static_cast<Object*>(parentModelIndex.internalPointer());
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
