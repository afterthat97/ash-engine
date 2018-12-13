#pragma once

#include <Generic/Common.h>

class Camera {
public:
    Camera();
    Camera(QVector3D pos, QVector3D dir, QVector3D up);

    void reset();
    void moveForward(float steps);
    void moveRight(float steps);
    void moveUp(float steps);
    void turnLeft(float angle);
    void lookUp(float angle);

    QVector3D getPosition();
    QMatrix4x4 getViewMatrix();

private:
    QVector3D pos, dir, up;
};
