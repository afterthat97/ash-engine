#pragma once

#include <UI/CentralWidget/SceneTreeView.h>
#include <UI/CentralWidget/OpenGLWidget.h>
#include <UI/CentralWidget/PropertyWidget/EmptyPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/LightPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/MaterialPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/MeshPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/TexturePropertyWidget.h>

class MainWindow;

class CentralWidget: public QWidget {
public:
    CentralWidget(MainWindow* parent);
    ~CentralWidget();

    void resetPropertyWidget();
    void setPropertyWidget(Model* model);
    void setPropertyWidget(Light* light);
    void setPropertyWidget(Mesh* mesh);
    void setPropertyWidget(Material* material);
    void setPropertyWidget(Texture* texture);

    friend class MainWindow;
    friend class MainMenuBar;

private:
    MainWindow * mainWindow;
    SceneTreeView * sceneTreeView;
    OpenGLWidget * openGLWidget;
    QWidget * propertyWidget;
    QSplitter * splitter;
    QHBoxLayout * mainLayout;
};