QT += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masterEngine
TEMPLATE = app
VERSION = 1.0.0
PREVIEW = -rc3
DESTDIR = build/bin
MOC_DIR = build/tmp
OBJECTS_DIR = build/tmp
RCC_DIR = build/tmp
UI_DIR = build/tmp

CONFIG += warn_on

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    APP_VERSION=\\\"v$${VERSION}$${PREVIEW}\\\"

INCLUDEPATH += \
    include/Core \
    include/OpenGL \
    include/UI \
    3rdparty

RESOURCES += resources.qrc

macx {
    LIBS += -L$$PWD/lib/mac/ -lassimp
    ICON = resources/mac/AppIcon.icns
}

win32 {
    LIBS += -lopengl32
    LIBS += -L$$PWD/lib/win/ -lassimp-vc140-mt
    DLLDESTDIR = $$DESTDIR
    RC_ICONS = resources/win/AppIcon.ico
}

CONFIG(debug) {
    DEFINES += _DEBUG
}

HEADERS += \
    include/Core/AbstractEntity.h \
    include/Core/AbstractLight.h \
    include/Core/AmbientLight.h \
    include/Core/Axis.h \
    include/Core/Camera.h \
    include/Core/Common.h \
    include/Core/Core.h \
    include/Core/DirectionalLight.h \
    include/Core/Gridline.h \
    include/Core/Material.h \
    include/Core/Mesh.h \
    include/Core/Model.h \
    include/Core/ModelExporter.h \
    include/Core/ModelLoader.h \
    include/Core/PointLight.h \
    include/Core/Scene.h \
    include/Core/SceneLoader.h \
    include/Core/SceneSaver.h \
    include/Core/SpotLight.h \
    include/Core/Texture.h \
    include/Core/TextureLoader.h \
    include/Core/Vertex.h \
    include/OpenGL/Common.h \
    include/OpenGL/FPSCounter.h \
    include/OpenGL/OpenGL.h \
    include/OpenGL/OpenGLConfig.h \
    include/OpenGL/OpenGLMaterial.h \
    include/OpenGL/OpenGLMesh.h \
    include/OpenGL/OpenGLRenderer.h \
    include/OpenGL/OpenGLScene.h \
    include/OpenGL/OpenGLTexture.h \
    include/OpenGL/OpenGLUniformBufferObject.h \
    include/OpenGL/OpenGLWindow.h \
    include/UI/AmbientLightProperty.h \
    include/UI/CameraProperty.h \
    include/UI/Common.h \
    include/UI/DirectionalLightProperty.h \
    include/UI/FloatEdit.h \
    include/UI/FloatSlider.h \
    include/UI/GridlineProperty.h \
    include/UI/MainWindow.h \
    include/UI/MaterialProperty.h \
    include/UI/MeshProperty.h \
    include/UI/ModelProperty.h \
    include/UI/PointLightProperty.h \
    include/UI/SceneTreeWidget.h \
    include/UI/SpotLightProperty.h \
    include/UI/TextureProperty.h \
    include/UI/UI.h \
    include/UI/Vector3DEdit.h \
    include/UI/Vector3DEditSlider.h

SOURCES += \
    src/Core/AbstractEntity.cpp \
    src/Core/AbstractLight.cpp \
    src/Core/AmbientLight.cpp \
    src/Core/Axis.cpp \
    src/Core/Camera.cpp \
    src/Core/DirectionalLight.cpp \
    src/Core/Gridline.cpp \
    src/Core/Material.cpp \
    src/Core/Mesh.cpp \
    src/Core/Model.cpp \
    src/Core/ModelExporter.cpp \
    src/Core/ModelLoader.cpp \
    src/Core/PointLight.cpp \
    src/Core/Scene.cpp \
    src/Core/SceneLoader.cpp \
    src/Core/SceneSaver.cpp \
    src/Core/SpotLight.cpp \
    src/Core/Texture.cpp \
    src/Core/TextureLoader.cpp \
    src/Core/Vertex.cpp \
    src/OpenGL/FPSCounter.cpp \
    src/OpenGL/OpenGLConfig.cpp \
    src/OpenGL/OpenGLMaterial.cpp \
    src/OpenGL/OpenGLMesh.cpp \
    src/OpenGL/OpenGLRenderer.cpp \
    src/OpenGL/OpenGLScene.cpp \
    src/OpenGL/OpenGLTexture.cpp \
    src/OpenGL/OpenGLUniformBufferObject.cpp \
    src/OpenGL/OpenGLWindow.cpp \
    src/UI/AmbientLightProperty.cpp \
    src/UI/CameraProperty.cpp \
    src/UI/DirectionalLightProperty.cpp \
    src/UI/FloatEdit.cpp \
    src/UI/FloatSlider.cpp \
    src/UI/GridlineProperty.cpp \
    src/UI/MainWindow.cpp \
    src/UI/MaterialProperty.cpp \
    src/UI/MeshProperty.cpp \
    src/UI/ModelProperty.cpp \
    src/UI/PointLightProperty.cpp \
    src/UI/SceneTreeWidget.cpp \
    src/UI/SpotLightProperty.cpp \
    src/UI/TextureProperty.cpp \
    src/UI/Vector3DEdit.cpp \
    src/UI/Vector3DEditSlider.cpp \
    src/masterEngine.cpp

