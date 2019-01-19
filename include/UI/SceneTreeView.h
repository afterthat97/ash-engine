#pragma once

#include <UI/Common.h>

// TODO: Represent different types with icons.
// TODO: Support to select and delete objects.

class SceneTreeModel: public QAbstractItemModel {
    Q_OBJECT

public:
    SceneTreeModel(Scene* scene, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    Scene* m_host;
};

class SceneTreeView: public QTreeView {
    Q_OBJECT

public:
    SceneTreeView(Scene* scene, QWidget* parent = 0);

    void setScene(Scene* scene);

signals:
    void cameraSelected(Camera* camera);
    void gridlineSelected(Gridline* gridline);
    void ambientLightSelected(AmbientLight* light);
    void directionalLightSelected(DirectionalLight* light);
    void pointLightSelected(PointLight* light);
    void spotLightSelected(SpotLight* light);
    void modelSelected(Model* model);
    void meshSelected(Mesh* mesh);
    void materialSelected(Material* material);

    void cameraDeselected(Camera* camera);
    void gridlineDeselected(Gridline* gridline);
    void ambientLightDeselected(AmbientLight* light);
    void directionalLightDeselected(DirectionalLight* light);
    void pointLightDeselected(PointLight* light);
    void spotLightDeselected(SpotLight* light);
    void modelDeselected(Model* model);
    void meshDeselected(Mesh* mesh);
    void materialDeselected(Material* material);

private:
    Scene* m_host;

private slots:
    void selectionModelChanged(const QModelIndex & cnt, const QModelIndex & prev);
    void hostDestroyed(QObject* host);
};
