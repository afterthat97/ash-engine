#pragma once

#include <OpenGL/Common.h>

template<class CoreClass, class OpenGLClass>
class OpenGLManager {
public:
    OpenGLManager() {}

    static OpenGLManager* currentManager() {
        static OpenGLManager* manager = new OpenGLManager;
        return manager;
    }

    OpenGLClass* getOpenGLObject(CoreClass* coreObject) {
        if (m_coreClassToOpenGLClass.find(coreObject) == m_coreClassToOpenGLClass.end()) {
            OpenGLClass* openGLObject = new OpenGLClass(coreObject);
            m_coreClassToOpenGLClass[coreObject] = openGLObject;
        }
        return m_coreClassToOpenGLClass[coreObject];
    }

    bool removeOpenGLObject(CoreClass* coreObject) {
        if (m_coreClassToOpenGLClass.find(coreObject) != m_coreClassToOpenGLClass.end()) {
            m_coreClassToOpenGLClass.remove(coreObject);
            return true;
        }
        return false;
    }

private:
    QHash<CoreClass*, OpenGLClass*> m_coreClassToOpenGLClass;
};
