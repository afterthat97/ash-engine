#include <Core/Camera.h>

Camera::Camera(QObject* parent): QObject(parent) {
    reset();
}

Camera::Camera(QVector3D position, QVector3D direction, QObject* parent): QObject(parent) {
    setMovingSpeed(0.1f);
    setFieldOfView(45.0f);
    setNearPlane(0.1f);
    setFarPlane(100000.0f);
    setPosition(position);
    setDirection(direction);
}

Camera::Camera(const Camera & camera) {
    m_movingSpeed = camera.m_movingSpeed;
    m_fieldOfView = camera.m_fieldOfView;
    m_aspectRatio = camera.m_aspectRatio;
    m_nearPlane = camera.m_nearPlane;
    m_farPlane = camera.m_farPlane;
    m_position = camera.m_position;
    m_direction = camera.m_direction;
    m_up = camera.m_up;
}

void Camera::moveForward(float shift) {
    setPosition(m_position + m_direction * shift * m_movingSpeed);
}

void Camera::moveRight(float shift) {
    setPosition(m_position + QVector3D::crossProduct(m_direction, m_up) * shift * m_movingSpeed);
}

void Camera::moveUp(float shift) {
    setPosition(m_position + m_up * shift* m_movingSpeed);
}

void Camera::turnLeft(float angle) {
    QMatrix4x4 mat;
    mat.rotate(angle, QVector3D(0, 1, 0));
    setDirection(mat * m_direction);
}

void Camera::lookUp(float angle) {
    QMatrix4x4 mat;
    mat.rotate(angle, QVector3D::crossProduct(m_direction, m_up));
    setDirection(mat * m_direction);
}

void Camera::dumpObjectInfo(int l) {
    qDebug().nospace() << tab(l) << "Camera: " << objectName();
    qDebug().nospace() << tab(l + 1) << "Position:      " << m_position;
    qDebug().nospace() << tab(l + 1) << "Direction:     " << m_direction;
    qDebug().nospace() << tab(l + 1) << "Up:            " << m_up;
    qDebug().nospace() << tab(l + 1) << "Moving Speed:  " << m_movingSpeed;
    qDebug().nospace() << tab(l + 1) << "Field of View: " << m_fieldOfView;
    qDebug().nospace() << tab(l + 1) << "Aspect Ratio:  " << m_aspectRatio;
    qDebug().nospace() << tab(l + 1) << "Near Plane:    " << m_nearPlane;
    qDebug().nospace() << tab(l + 1) << "Far Plane:     " << m_farPlane;
}

void Camera::dumpObjectTree(int l) {
    dumpObjectInfo(l);
}

// Get properties

float Camera::movingSpeed() const {
    return m_movingSpeed;
}

float Camera::fieldOfView() const {
    return m_fieldOfView;
}

float Camera::aspectRatio() const {
    return m_aspectRatio;
}

float Camera::nearPlane() const {
    return m_nearPlane;
}

float Camera::farPlane() const {
    return m_farPlane;
}

QVector3D Camera::position() const {
    return m_position;
}

QVector3D Camera::direction() const {
    return m_direction;
}

// Get projection and view matrix

QMatrix4x4 Camera::projectionMatrix() const {
    QMatrix4x4 mat;
    mat.perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
    return mat;
}

QMatrix4x4 Camera::viewMatrix() const {
    QMatrix4x4 mat;
    mat.lookAt(m_position, m_position + m_direction, m_up);
    return mat;
}

// Public slots

void Camera::reset() {
    setMovingSpeed(0.1f);
    setFieldOfView(45.0f);
    setNearPlane(0.1f);
    setFarPlane(100000.0f);
    setPosition(QVector3D(0, 2, 10));
    setDirection(QVector3D(0, 0, -1));
}

void Camera::setMovingSpeed(float movingSpeed) {
    if (!qFuzzyCompare(m_movingSpeed, movingSpeed)) {
        m_movingSpeed = movingSpeed;
        movingSpeedChanged(m_movingSpeed);
    }
}

void Camera::setFieldOfView(float fieldOfView) {
    if (!qFuzzyCompare(m_fieldOfView, fieldOfView)) {
        m_fieldOfView = fieldOfView;
        fieldOfViewChanged(m_fieldOfView);
    }
}

void Camera::setAspectRatio(float aspectRatio) {
    if (!qFuzzyCompare(m_aspectRatio, aspectRatio)) {
        m_aspectRatio = aspectRatio;
        aspectRatioChanged(m_aspectRatio);
    }
}

void Camera::setNearPlane(float nearPlane) {
    if (!qFuzzyCompare(m_nearPlane, nearPlane)) {
        m_nearPlane = nearPlane;
        nearPlaneChanged(m_nearPlane);
    }
}

void Camera::setFarPlane(float farPlane) {
    if (!qFuzzyCompare(m_farPlane, farPlane)) {
        m_farPlane = farPlane;
        farPlaneChanged(m_farPlane);
    }
}

void Camera::setPosition(QVector3D position) {
    if (!qFuzzyCompare(m_position, position)) {
        m_position = position;
        positionChanged(m_position);
    }
}

void Camera::setDirection(QVector3D direction) {
    direction.normalize();
    if (!qFuzzyCompare(m_direction, direction)) {
        m_direction = direction;
        setUpVector();
        directionChanged(m_direction);
    }
}

// Private functions

void Camera::setUpVector() {
    QVector3D t = QVector3D::crossProduct(m_direction, QVector3D(0, 1, 0));
    m_up = QVector3D::crossProduct(t, m_direction);
}
