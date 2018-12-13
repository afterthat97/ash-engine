QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masterEngine
TEMPLATE = app
VERSION = v1.0.0-rc1

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    APP_VERSION=\\\"$${VERSION}\\\"

INCLUDEPATH += \
    include \
    3rdparty

RESOURCES += \
    resources.qrc

macx {
    LIBS += -L$$PWD/lib/mac/ -lassimp
    ICON = resources/mac/AppIcon.icns
    QMAKE_INFO_PLIST = resources/mac/Info.plist
}

win32 {
    LIBS += -L$$PWD/lib/win/ -lassimp-vc140-mt
    RC_ICONS = resources/win/AppIcon.ico
}

HEADERS += \
    include/Generic/Allocator.h \
    include/Generic/Camera.h \
    include/Generic/Common.h \
    include/Generic/Gridline.h \
    include/Generic/Helper.h \
    include/Generic/Light.h \
    include/Generic/Material.h \
    include/Generic/Mesh.h \
    include/Generic/Model.h \
    include/Generic/Object.h \
    include/Generic/Scene.h \
    include/Generic/Texture.h \
    include/Generic/Vertex.h \
    include/IO/Loader.h \
    include/OpenGL/Common.h \
    include/OpenGL/OpenGLConfig.h \
    include/OpenGL/OpenGLManager.h \
    include/OpenGL/OpenGLMesh.h \
    include/OpenGL/OpenGLRenderer.h \
    include/OpenGL/OpenGLTexture.h \
    include/OpenGL/OpenGLWidget.h \
    include/UI/Common.h \
    include/UI/LightPropertyWidget.h \
    include/UI/MainMenuBar.h \
    include/UI/MainWindow.h \
    include/UI/MaterialPropertyWidget.h \
    include/UI/MeshPropertyWidget.h \
    include/UI/SceneTreeView.h \
    include/UI/TexturePropertyWidget.h \
    include/UI/Vector3DWidget.h \

SOURCES += \
    src/Generic/Allocator.cpp \
    src/Generic/Camera.cpp \
    src/Generic/Gridline.cpp \
    src/Generic/Helper.cpp \
    src/Generic/Light.cpp \
    src/Generic/Material.cpp \
    src/Generic/Mesh.cpp \
    src/Generic/Model.cpp \
    src/Generic/Object.cpp \
    src/Generic/Scene.cpp \
    src/Generic/Texture.cpp \
    src/IO/Loader.cpp \
    src/OpenGL/OpenGLConfig.cpp \
    src/OpenGL/OpenGLManager.cpp \
    src/OpenGL/OpenGLMesh.cpp \
    src/OpenGL/OpenGLRenderer.cpp \
    src/OpenGL/OpenGLTexture.cpp \
    src/OpenGL/OpenGLWidget.cpp \
    src/UI/LightPropertyWidget.cpp \
    src/UI/MainMenuBar.cpp \
    src/UI/MainWindow.cpp \
    src/UI/MaterialPropertyWidget.cpp \
    src/UI/MeshPropertyWidget.cpp \
    src/UI/SceneTreeView.cpp \
    src/UI/TexturePropertyWidget.cpp \
    src/UI/Vector3DWidget.cpp \
    src/masterEngine.cpp
