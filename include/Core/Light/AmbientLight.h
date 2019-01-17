#pragma once

#include <Core/Light/AbstractLight.h>

class AmbientLight: public AbstractLight {
    Q_OBJECT

public:
    AmbientLight(QVector3D color = QVector3D(1, 1, 1), QObject* parent = 0);
    // TODO: Copy constructor

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;
};
