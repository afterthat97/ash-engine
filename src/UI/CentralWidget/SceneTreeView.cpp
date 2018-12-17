#include <UI/CentralWidget/SceneTreeView.h>
#include <UI/MainWindow.h>

SceneTreeView::SceneTreeView(QWidget* parent): QTreeView(parent) {
    sceneTreeModel = new SceneTreeModel;
    setModel(sceneTreeModel);
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(selectionModelChanged(QModelIndex, QModelIndex)));
}

SceneTreeView::~SceneTreeView() {
    delete sceneTreeModel;
}

void SceneTreeView::selectionModelChanged(const QModelIndex & cntIndex, const QModelIndex & prevIndex) {
    if (prevIndex.isValid()) {
        QObject * pointer = static_cast<QObject*>(prevIndex.internalPointer());
        if (Camera* camera = dynamic_cast<Camera*>(pointer)) {
            cameraSelected(camera, false);
        } else if (Model* model = dynamic_cast<Model*>(pointer)) {
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
        if (Camera* camera = dynamic_cast<Camera*>(pointer)) {
            cameraSelected(camera, true);
        } else if (Model* model = dynamic_cast<Model*>(pointer)) {
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

// Return data for display
QVariant SceneTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    QObject * pointer = static_cast<QObject*>(index.internalPointer());
    if (Camera* camera = dynamic_cast<Camera*>(pointer)) {
        return QVariant("Camera");
    } else if (Model* item = dynamic_cast<Model*>(pointer)) {
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

// Return header data
QVariant SceneTreeModel::headerData(int, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(Scene::currentScene()->objectName());
    return QVariant();
}

// Get the row(th) child of parentModelIndex
QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parentModelIndex) const {
    if (!hasIndex(row, column, parentModelIndex)) return QModelIndex();

    QObject * parentPointer = static_cast<QObject*>(parentModelIndex.internalPointer());
    if (!parentModelIndex.isValid()) {
        Scene* parentItem = Scene::currentScene();
        if (row == 0) // Camera
            return createIndex(row, column, parentItem->getCamera());
        else if (row <= (int) parentItem->getLights().size()) // Lights
            return createIndex(row, column, parentItem->getLights()[row - 1]);
        else // Models
            return createIndex(row, column, parentItem->getModels()[row - parentItem->getLights().size() - 1]);
    } else if (Model* parentItem = dynamic_cast<Model*>(parentPointer)) {
        if (row < (int) parentItem->getMeshes().size()) // Meshes
            return createIndex(row, column, parentItem->getMeshes()[row]);
        else // Child models
            return createIndex(row, column, parentItem->getChildren()[row - parentItem->getMeshes().size()]);
    } else if (Mesh* parentItem = dynamic_cast<Mesh*>(parentPointer)) {
        return createIndex(row, column, parentItem->getMaterial()); // Material
    } else if (Material* parentItem = dynamic_cast<Material*>(parentPointer)) {
        return createIndex(row, column, parentItem->getTextures()[row]); // Textures
    }
    return QModelIndex();
}

// Return the ModelIndex of parent
QModelIndex SceneTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();

    QObject * parentPointer = static_cast<QObject*>(index.internalPointer())->parent();
    if (!parentPointer || parentPointer == Scene::currentScene()) return QModelIndex();

    QObject * grandParentPointer = parentPointer->parent();
    if (Scene* scene = dynamic_cast<Scene*>(grandParentPointer)) {
        // Parent pointer must be Model*
        for (uint32_t i = 0; i < scene->getModels().size(); i++)
            if (scene->getModels()[i] == parentPointer)
                return createIndex(int(i + scene->getLights().size() + 1), 0, parentPointer);
    } else if (Model* model = dynamic_cast<Model*>(grandParentPointer)) {
        // Parent pointer could be Mesh* or Model*
        for (uint32_t i = 0; i < model->getMeshes().size(); i++)
            if (model->getMeshes()[i] == parentPointer)
                return createIndex(int(i), 0, parentPointer);
        for (uint32_t i = 0; i < model->getChildren().size(); i++)
            if (model->getChildren()[i] == parentPointer)
                return createIndex(int(i + model->getMeshes().size()), 0, parentPointer);
    } else if (Mesh* mesh = dynamic_cast<Mesh*>(grandParentPointer)) {
        // Parent pointer must be Material*
        return createIndex(0, 0, parentPointer);
    }
    return QModelIndex();
}

// Return the number of children
int SceneTreeModel::rowCount(const QModelIndex &parentModelIndex) const {
    QObject * parentPointer = static_cast<QObject*>(parentModelIndex.internalPointer());
    if (!parentModelIndex.isValid()) { // Scene
        // Num = Camera + Lights + Models
        Scene* parentItem = Scene::currentScene();
        return int(parentItem->getLights().size() + parentItem->getModels().size() + 1);
    } else if (Model* parentItem = dynamic_cast<Model*>(parentPointer)) {
        // Num = Child models + Meshes
        return int(parentItem->getChildren().size() + parentItem->getMeshes().size());
    } else if (Light* parentItem = dynamic_cast<Light*>(parentPointer)) {
        // Light has no children
        return 0;
    } else if (Mesh* parentItem = dynamic_cast<Mesh*>(parentPointer)) {
        // Num = 1 (if it has Material)
        return int(parentItem->getMaterial() != NULL);
    } else if (Material* parentItem = dynamic_cast<Material*>(parentPointer)) {
        // Num = Textures
        return int(parentItem->getTextures().size());
    }
    return 0;
}
