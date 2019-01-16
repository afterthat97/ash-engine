#pragma once

#include <Core/Light/AbstractLight.h>

class DirectionalLight: public AbstractLight {
    Q_OBJECT

public:
    DirectionalLight(QVector3D color = QVector3D(1, 1, 1), QVector3D direction = QVector3D(-1, -1, -1), QObject* parent = 0);

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;

    QVector3D direction();

public slots:
    void setDirection(QVector3D direction);

signals:
    void directionChanged(QVector3D direction);

protected:
    QVector3D m_direction;
};