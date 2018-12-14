#pragma once

#include <UI/Common.h>
#include <Generic/Scene.h>

class MainWindow;
class SceneTreeModel;

class SceneTreeView: public QTreeView {
    Q_OBJECT

public:
    SceneTreeView(MainWindow* mainWindow, QWidget* parent = Q_NULLPTR);
    ~SceneTreeView();

signals:
    void modelSelected(Model* model, bool selected);
    void lightSelected(Light* light, bool selected);
    void meshSelected(Mesh* mesh, bool selected);
    void materialSelected(Material* material, bool selected);
    void textureSelected(Texture* texture, bool selected);

private slots:
    void selectionChanged(const QModelIndex & cnt, const QModelIndex & prev);

private:
    MainWindow * mainWindow;
    SceneTreeModel * sceneTreeModel;
};

class SceneTreeModel: public QAbstractItemModel {
    Q_OBJECT

public:
    SceneTreeModel(QObject *parent = 0);
    ~SceneTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
};
