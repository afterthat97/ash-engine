#include <Generic/Object.h>

Object::Object() {
    parent = NULL;
}

Object::~Object() {}

void Object::setName(QString _name) {
    if (_name == "") return;
    name = _name;
}

void Object::setParent(Object * _parent) {
    parent = _parent;
}

QString Object::getName() {
    return name;
}

Object * Object::getParent() {
    return parent;
}

uint32_t Object::getID() {
    return id;
}

void Object::initID() {
    id = -1;
}
