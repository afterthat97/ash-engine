#pragma once

#include <Core/Mesh.h>

class Axis: public AbstractEntity {
    Q_OBJECT

public:
    enum AxisType {
        Translate = 0,
        Rotate = 3,
        Scale = 6
    };

    enum TransformationMode {
        None = 10,
        Translate_X = 11,
        Translate_Y = 12,
        Translate_Z = 13,
        Rotate_X = 14,
        Rotate_Y = 15,
        Rotate_Z = 16,
        Scale_X = 17,
        Scale_Y = 18,
        Scale_Z = 19
    };

    Axis(QObject * parent = 0);
    ~Axis();

    void dumpObjectInfo(int level = 0) override;
    void dumpObjectTree(int level = 0) override;

    AxisType axisType() const;
    TransformationMode transformMode() const;
    QVector<Mesh*>& markers();

    void drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view);
    
    void bindTo(AbstractEntity* host) override;
    void unbind() override;

public slots:
    void setAxisType(AxisType axisType);
    void setTransformMode(TransformationMode mode);

private:
    AxisType m_axisType;
    TransformationMode m_mode;
    QVector<Mesh*> m_markers;
};