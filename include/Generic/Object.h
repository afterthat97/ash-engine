#pragma once

#include <Generic/Common.h>

class Object {
public:
    Object();
    ~Object();

    void setName(QString name);
    void setParent(Object* parent);

    QString getName();
    Object* getParent();
    uint32_t getID();

protected:
    QString name;
    uint32_t id;
    Object* parent;

    virtual void initID();
};
