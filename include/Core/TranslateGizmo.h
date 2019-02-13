#pragma once

#include <AbstractGizmo.h>

class TranslateGizmo: public AbstractGizmo {
    Q_OBJECT

public:
    TranslateGizmo(QObject* parent = 0);
    ~TranslateGizmo();

    void translate(QVector3D delta) override;
    void rotate(QQuaternion rotation) override;
    void rotate(QVector3D rotation) override;
    void scale(QVector3D scaling) override;

    QVector3D position() const override;
    QVector3D rotation() const override;
    QVector3D scaling() const override;

    QMatrix4x4 globalSpaceMatrix() const;
    QMatrix4x4 globalModelMatrix() const override;

    void drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view) override;

public slots:
    void setPosition(QVector3D position) override;
    void setRotation(QQuaternion rotation) override;
    void setRotation(QVector3D rotation) override;
    void setScaling(QVector3D scaling) override;

};