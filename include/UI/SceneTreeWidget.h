#pragma once

#include <UI/Common.h>

class SceneTreeWidget: public QTreeWidget {
    Q_OBJECT

public:
    SceneTreeWidget(Scene* scene, QWidget* parent = 0);

    void setScene(Scene* scene);

public slots:
    void reload();

protected:
    void keyPressEvent(QKeyEvent *e) override;

signals:
    void itemSelected(QVariant item);
    void itemDeselected(QVariant item);

private:
    Scene* m_host;

    QTreeWidgetItem* createModelItem(QTreeWidgetItem* parent, Model* model);

private slots:
    void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void hostDestroyed(QObject* host);
};
