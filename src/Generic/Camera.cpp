#include <Generic/Camera.h>

Camera::Camera() {
    pos = QVector3D(0.0f, 50.0f, 300.0f);
    dir = QVector3D(0.0f, 0.0f, -1.0f);
    up = QVector3D(0.0f, 1.0f, 0.0f);
}

Camera::Camera(QVector3D _pos, QVector3D _dir, QVector3D _up)
    :pos(_pos), dir(_dir), up(_up) {
    dir.normalize();
    up.normalize();
}

void Camera::reset() {
    pos = QVector3D(0.0f, 50.0f, 300.0f);
    dir = QVector3D(0.0f, 0.0f, -1.0f);
    up = QVector3D(0.0f, 1.0f, 0.0f);
}

void Camera::moveForward(float steps) {
    pos = pos + dir * steps;
}

void Camera::moveRight(float steps) {
    pos = pos + QVector3D::crossProduct(dir, up) * steps;
}

void Camera::moveUp(float steps) {
    pos = pos + up * steps;
}

void Camera::turnLeft(float angle) {
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.rotate(angle, QVector3D(0.0, 1.0, 0.0));
    dir = mat * dir;
    up = mat * up;
}

void Camera::lookUp(float angle) {
    QVector3D t = QVector3D::crossProduct(dir, up);
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.rotate(angle, t);
    dir = mat * dir;
    up = mat * up;
}

QVector3D Camera::getPosition() {
    return pos;
}

QMatrix4x4 Camera::getViewMatrix() {
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.lookAt(pos, pos + dir, up);
    return mat;
}
