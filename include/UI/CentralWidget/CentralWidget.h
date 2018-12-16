#pragma once

#include <UI/CentralWidget/SceneTreeView.h>
#include <UI/CentralWidget/OpenGLWidget.h>
#include <UI/CentralWidget/PropertyWidget/ScenePropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/LightPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/MaterialPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/MeshPropertyWidget.h>
#include <UI/CentralWidget/PropertyWidget/TexturePropertyWidget.h>

class MainWindow;

class CentralWidget: public QWidget {
    Q_OBJECT

public:
    CentralWidget(MainWindow* parent);
    ~CentralWidget();

    // MARK
    void resetPropertyWidget();

    friend class MainWindow;
    friend class MainMenuBar;

private:
    MainWindow * mainWindow;
    SceneTreeView * sceneTreeView;
    OpenGLWidget * openGLWidget;
    QWidget * propertyWidget;
    QSplitter * splitter;
    QHBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void FPSChanged(uint32_t FPS);
    void modelSelected(Model* model, bool selected);
    void lightSelected(Light* light, bool selected);
    void meshSelected(Mesh* mesh, bool selected);
    void materialSelected(Material* material, bool selected);
    void textureSelected(Texture* texture, bool selected);
};