#pragma once

#include <TranslateGizmo.h>
#include <RotateGizmo.h>
#include <ScaleGizmo.h>

class TransformGizmo: public AbstractGizmo {
    Q_OBJECT

public:
    enum TransformMode {
        Translate,
        Rotate,
        Scale
    };

    TransformGizmo(QObject* parent = 0);
    ~TransformGizmo();

    void translate(QVector3D delta) override;
    void rotate(QQuaternion rotation) override;
    void rotate(QVector3D rotation) override;
    void scale(QVector3D scaling) override;

    QVector3D position() const override;
    QVector3D rotation() const override;
    QVector3D scaling() const override;

    QMatrix4x4 globalModelMatrix() const override;

    TransformAxis transformAxis() const override;
    TransformMode transformMode() const;

    QVector<Mesh*>& markers() override;
    void drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view) override;

    void bindTo(AbstractEntity* host) override;
    void unbind() override;

public slots:
    void setTransformAxis(TransformAxis axis) override;
    void setTransformMode(TransformMode mode);
    void setPosition(QVector3D position) override;
    void setRotation(QQuaternion rotation) override;
    void setRotation(QVector3D rotation) override;
    void setScaling(QVector3D scaling) override;

private:
    TranslateGizmo* m_translateGizmo;
    RotateGizmo* m_rotateGizmo;
    ScaleGizmo* m_scaleGizmo;
    AbstractGizmo* m_activatedGizmo;
};
