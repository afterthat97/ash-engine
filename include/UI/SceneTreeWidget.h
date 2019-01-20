#pragma once

#include <UI/Common.h>

// TODO: Represent different types with icons.
// TODO: Support to select and delete objects.

class SceneTreeWidget: public QTreeWidget {
    Q_OBJECT

public:
    SceneTreeWidget(Scene* scene, QWidget* parent = 0);

    void setScene(Scene* scene);
    void reload();

signals:
    void itemSelected(QVariant item);
    void itemDeselected(QVariant item);

private:
    Scene* m_host;

    QTreeWidgetItem* createModelItem(QTreeWidgetItem* parent, Model* model);

private slots:
    void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void hostChanged();
    void hostDestroyed(QObject* host);
};
