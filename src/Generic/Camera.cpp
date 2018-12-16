#include <Generic/Camera.h>

Camera::Camera(QVector3D _position, QVector3D _direction, QObject* parent): QObject(parent) {
    movingSpeed = 0.1f;
    fieldOfView = 45.0f;
    aspectRatio = 16.0f / 9.0f;
    nearPlane = 0.1f;
    farPlane = 100000.0f;
    position = _position;
    direction = _direction;
    direction.normalize();
    setUpVector();
}

// Move around functions

void Camera::moveForward(float shift) {
    position = position + direction * shift * movingSpeed;
    positionChanged(position);
}

void Camera::moveRight(float shift) {
    position = position + QVector3D::crossProduct(direction, up) * shift* movingSpeed;
    positionChanged(position);
}

void Camera::moveUp(float shift) {
    position = position + up * shift* movingSpeed;
    positionChanged(position);
}

void Camera::turnLeft(float angle) {
    QMatrix4x4 mat;
    mat.rotate(angle, QVector3D(0, 1, 0));
    direction = mat * direction;
    setUpVector();
    directionChanged(direction);
}

void Camera::lookUp(float angle) {
    QVector3D t = QVector3D::crossProduct(direction, up);
    QMatrix4x4 mat;
    mat.rotate(angle, t);
    direction = mat * direction;
    setUpVector();
    directionChanged(direction);
}

// Get properties

float Camera::getMovingSpeed() {
    return movingSpeed;
}

float Camera::getFieldOfView() {
    return fieldOfView;
}

float Camera::getAspectRatio() {
    return aspectRatio;
}

float Camera::getNearPlane() {
    return nearPlane;
}

float Camera::getFarPlane() {
    return farPlane;
}

QVector3D Camera::getPosition() {
    return position;
}

QVector3D Camera::getDirection() {
    return direction;
}

// Get projection and view matrix

QMatrix4x4 Camera::getProjectionMatrix() {
    QMatrix4x4 mat;
    mat.perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    return mat;
}

QMatrix4x4 Camera::getViewMatrix() {
    QMatrix4x4 mat;
    mat.lookAt(position, position + direction, up);
    return mat;
}

// Public slots

void Camera::reset() {
    movingSpeed = 0.1f;
    position = QVector3D(0.0f, 3.0f, 5.0f);
    direction = QVector3D(0.0f, 0.0f, -1.0f);
    setUpVector();
    positionChanged(position);
    directionChanged(direction);
}

void Camera::setMovingSpeed(float _movingSpeed) {
    movingSpeed = _movingSpeed;
    movingSpeedChanged(movingSpeed);
}

void Camera::setFieldOfView(float _fieldOfView) {
    fieldOfView = _fieldOfView;
    fieldOfViewChanged(fieldOfView);
}

void Camera::setAspectRatio(float _aspectRatio) {
    aspectRatio = _aspectRatio;
    aspectRatioChanged(aspectRatio);
}

void Camera::setNearPlane(float _nearPlane) {
    nearPlane = _nearPlane;
    nearPlaneChanged(nearPlane);
}

void Camera::setFarPlane(float _farPlane) {
    farPlane = _farPlane;
    farPlaneChanged(farPlane);
}

void Camera::setPosition(QVector3D _position) {
    position = _position;
    positionChanged(position);
}

void Camera::setDirection(QVector3D _direction) {
    direction = _direction;
    setUpVector();
    directionChanged(direction);
}

// Private functions

void Camera::setUpVector() {
    QVector3D t = QVector3D::crossProduct(direction, QVector3D(0, 1, 0));
    up = QVector3D::crossProduct(t, direction);
}
