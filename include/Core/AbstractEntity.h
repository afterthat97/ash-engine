#pragma once

#include <Common.h>

class AbstractEntity: public QObject {
    Q_OBJECT

public:
    AbstractEntity(QObject * parent = 0);
    AbstractEntity(const AbstractEntity& abstractObject3D);
    ~AbstractEntity();

    virtual void translate(QVector3D delta);
    virtual void rotate(QQuaternion rotation);
    virtual void rotate(QVector3D rotation);
    virtual void scale(QVector3D scaling);

    virtual void dumpObjectInfo(int level = 0) = 0;
    virtual void dumpObjectTree(int level = 0) = 0;

    bool visible() const;
    bool highlighted() const;
    bool selected() const;
    bool wireFrameMode() const;

    virtual bool isGizmo() const = 0;
    virtual bool isLight() const = 0;
    virtual bool isMesh() const = 0;
    virtual bool isModel() const = 0;

    virtual QVector3D position() const;
    virtual QVector3D rotation() const;
    virtual QVector3D scaling() const;

    virtual QMatrix4x4 localModelMatrix() const;
    virtual QMatrix4x4 globalModelMatrix() const;

    static AbstractEntity* getHighlighted();
    static AbstractEntity* getSelected();

public slots:
    void setVisible(bool visible);
    void setHighlighted(bool highlighted);
    void setSelected(bool selected);
    void setWireFrameMode(bool enabled);

    virtual void setPosition(QVector3D position);
    virtual void setRotation(QQuaternion rotation);
    virtual void setRotation(QVector3D rotation);
    virtual void setScaling(QVector3D scaling);

signals:
    void visibleChanged(bool visible);
    void highlightedChanged(bool highlighted);
    void selectedChanged(bool selected);
    void wireFrameModeChanged(bool enabled);

    void positionChanged(QVector3D position);
    void rotationChanged(QVector3D rotation);
    void scalingChanged(QVector3D scaling);

protected:
    bool m_visible, m_highlighted, m_selected, m_wireFrameMode;
    QVector3D m_position, m_rotation, m_scaling;

    static AbstractEntity *m_highlightedObject, *m_selectedObject;
};
