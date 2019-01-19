#pragma once

#include <Core/Light/AbstractLight.h>

class AmbientLight: public AbstractLight {
    Q_OBJECT

public:
    AmbientLight(QObject* parent = 0);
    AmbientLight(QVector3D color, QObject* parent = 0);
    AmbientLight(const AmbientLight& light);

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;
};
