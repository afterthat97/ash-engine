QT += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masterEngine
TEMPLATE = app
VERSION = 1.0.0
PREVIEW = -rc3

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
    include/Core/Light/AbstractLight.h \
    include/Core/Light/AmbientLight.h \
    include/Core/Light/DirectionalLight.h \
    include/Core/Light/PointLight.h \
    include/Core/Light/SpotLight.h \
    include/Core/Camera.h \
    include/Core/Common.h \
    include/Core/Core.h \
    include/Core/Gridline.h \
    include/Core/Material.h \
    include/Core/Mesh.h \
    include/Core/Model.h \
    include/Core/Scene.h \
    include/Core/Texture.h \
    include/Core/Vertex.h \
    include/IO/IO.h \
    include/IO/ModelLoader.h \
    include/IO/SceneLoader.h \
    include/IO/SceneSaver.h \
    include/IO/TextureLoader.h \
    include/OpenGL/Common.h \
    include/OpenGL/FPSCounter.h \
    include/OpenGL/OpenGL.h \
    include/OpenGL/OpenGLAmbientLight.h \
    include/OpenGL/OpenGLConfig.h \
    include/OpenGL/OpenGLDirectionalLight.h \
    include/OpenGL/OpenGLGridline.h \
    include/OpenGL/OpenGLManager.h \
    include/OpenGL/OpenGLMaterial.h \
    include/OpenGL/OpenGLMesh.h \
    include/OpenGL/OpenGLModel.h \
    include/OpenGL/OpenGLPointLight.h \
    include/OpenGL/OpenGLRenderer.h \
    include/OpenGL/OpenGLScene.h \
    include/OpenGL/OpenGLSpotLight.h \
    include/OpenGL/OpenGLTexture.h \
    include/OpenGL/OpenGLWidget.h \
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
    src/Core/Light/AbstractLight.cpp \
    src/Core/Light/AmbientLight.cpp \
    src/Core/Light/DirectionalLight.cpp \
    src/Core/Light/PointLight.cpp \
    src/Core/Light/SpotLight.cpp \
    src/Core/Camera.cpp \
    src/Core/Gridline.cpp \
    src/Core/Material.cpp \
    src/Core/Mesh.cpp \
    src/Core/Model.cpp \
    src/Core/Scene.cpp \
    src/Core/Texture.cpp \
    src/Core/Vertex.cpp \
    src/IO/ModelLoader.cpp \
    src/IO/SceneLoader.cpp \
    src/IO/SceneSaver.cpp \
    src/IO/TextureLoader.cpp \
    src/OpenGL/FPSCounter.cpp \
    src/OpenGL/OpenGLAmbientLight.cpp \
    src/OpenGL/OpenGLConfig.cpp \
    src/OpenGL/OpenGLDirectionalLight.cpp \
    src/OpenGL/OpenGLGridline.cpp \
    src/OpenGL/OpenGLManager.cpp \
    src/OpenGL/OpenGLMaterial.cpp \
    src/OpenGL/OpenGLMesh.cpp \
    src/OpenGL/OpenGLModel.cpp \
    src/OpenGL/OpenGLPointLight.cpp \
    src/OpenGL/OpenGLRenderer.cpp \
    src/OpenGL/OpenGLScene.cpp \
    src/OpenGL/OpenGLSpotLight.cpp \
    src/OpenGL/OpenGLTexture.cpp \
    src/OpenGL/OpenGLWidget.cpp \
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
