#pragma once

#include <UI/Common.h>
#include <Generic/Scene.h>

class SceneTreeView: public QTreeView {
public:
    SceneTreeView(QWidget* parent = Q_NULLPTR);
    ~SceneTreeView();
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
