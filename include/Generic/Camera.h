#pragma once

#include <Generic/Common.h>

class Camera: public QObject {
    Q_OBJECT

public:
    Camera(QVector3D position, QVector3D direction, QObject* parent = 0);

    void moveForward(float shift);
    void moveRight(float shift);
    void moveUp(float shift);
    void turnLeft(float angle);
    void lookUp(float angle);

    float getMovingSpeed();
    float getFieldOfView();
    float getAspectRatio();
    float getNearPlane();
    float getFarPlane();
    QVector3D getPosition();
    QVector3D getDirection();

    QMatrix4x4 getProjectionMatrix();
    QMatrix4x4 getViewMatrix();

public slots:
    void reset();
    void setMovingSpeed(float movingSpeed);
    void setFieldOfView(float fieldOfView);
    void setAspectRatio(float aspectRatio);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);
    void setPosition(QVector3D position);
    void setDirection(QVector3D direction);

signals:
    void movingSpeedChanged(float movingSpeed);
    void fieldOfViewChanged(float fieldOfView);
    void aspectRatioChanged(float aspectRatio);
    void nearPlaneChanged(float nearPlane);
    void farPlaneChanged(float farPlane);
    void positionChanged(QVector3D position);
    void directionChanged(QVector3D direction);

private:
    float movingSpeed, fieldOfView, aspectRatio, nearPlane, farPlane;
    QVector3D position, direction, up;

    void setUpVector();
};
