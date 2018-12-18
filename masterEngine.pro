QT += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masterEngine
TEMPLATE = app
VERSION = 1.0.0
PREVIEW = -rc1

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    APP_VERSION=\\\"v$${VERSION}$${PREVIEW}\\\"

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
    LIBS += -lopengl32
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
    include/UI/CentralWidget/PropertyWidget/FloatEditWidget.h \
    include/UI/CentralWidget/PropertyWidget/LightPropertyWidget.h \
    include/UI/CentralWidget/PropertyWidget/MaterialPropertyWidget.h \
    include/UI/CentralWidget/PropertyWidget/MeshPropertyWidget.h \
    include/UI/CentralWidget/PropertyWidget/ModelPropertyWidget.h \
    include/UI/CentralWidget/PropertyWidget/StringEditWidget.h \
    include/UI/CentralWidget/PropertyWidget/TexturePropertyWidget.h \
    include/UI/CentralWidget/PropertyWidget/Vector3DEditWidget.h \
    include/UI/CentralWidget/CentralWidget.h \
    include/UI/CentralWidget/OpenGLWidget.h \
    include/UI/CentralWidget/SceneTreeView.h \
    include/UI/Common.h \
    include/UI/MainMenuBar.h \
    include/UI/MainWindow.h \
    include/UI/CentralWidget/PropertyWidget/CameraPropertyWidget.h

SOURCES += \
    src/Generic/Allocator.cpp \
    src/Generic/Camera.cpp \
    src/Generic/Gridline.cpp \
    src/Generic/Helper.cpp \
    src/Generic/Light.cpp \
    src/Generic/Material.cpp \
    src/Generic/Mesh.cpp \
    src/Generic/Model.cpp \
    src/Generic/Scene.cpp \
    src/Generic/Texture.cpp \
    src/IO/Loader.cpp \
    src/OpenGL/OpenGLConfig.cpp \
    src/OpenGL/OpenGLManager.cpp \
    src/OpenGL/OpenGLMesh.cpp \
    src/OpenGL/OpenGLRenderer.cpp \
    src/OpenGL/OpenGLTexture.cpp \
    src/UI/CentralWidget/PropertyWidget/FloatEditWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/LightPropertyWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/MaterialPropertyWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/MeshPropertyWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/ModelPropertyWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/StringEditWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/TexturePropertyWidget.cpp \
    src/UI/CentralWidget/PropertyWidget/Vector3DEditWidget.cpp \
    src/UI/CentralWidget/CentralWidget.cpp \
    src/UI/CentralWidget/OpenGLWidget.cpp \
    src/UI/CentralWidget/SceneTreeView.cpp \
    src/UI/MainMenuBar.cpp \
    src/UI/MainWindow.cpp \
    src/masterEngine.cpp \
    src/UI/CentralWidget/PropertyWidget/CameraPropertyWidget.cpp

