#pragma once

#include <UI/Common.h>
#include <UI/SceneTreeWidget.h>
#include <OpenGL/OpenGLWidget.h>

// TODO: Find a better way to manage property widgets.

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Scene * scene, QWidget *parent = 0);

private:
    Scene *m_host;

    QMenuBar *m_menuBar;
    QWidget* m_centralWidget;
    QSplitter * m_splitter;
    QStatusBar * m_statusBar;

    SceneTreeWidget *m_sceneTreeView;
    OpenGLWindow *m_openGLWindow;
    QWidget *m_propertyWidget;

    QAction *actionFileNew, *actionFileOpen, *actionFileExit;
    QAction *actionCreateGridline;
    QAction *actionCreateAmbientLight, *actionCreateDirectionalLight, *actionCreatePointLight, *actionCreateSpotLight;
    QAction *actionCreateBasicCone, *actionCreateBasicCube, *actionCreateBasicCylinder, *actionCreateBasicPlane, *actionCreateBasicSphere;
    QAction *actionHelpAbout, *actionHelpCheckUpdate;

    void configMenu();
    void configLayout();
    void configSignals();

private slots:
    void fpsChanged(int fps);
    void itemSelected(QVariant item);

    void fileNew();
    void fileOpen();
    void fileExit();

    void createGridline();

    void createAmbientLight();
    void createDirectionalLight();
    void createPointLight();
    void createSpotLight();
    
    void createBasicCone();
    void createBasicCube();
    void createBasicCylinder();
    void createBasicPlane();
    void createBasicSphere();
    
    void helpAbout();
    void helpCheckForUpdates();
    void replyOfUpdates(QNetworkReply* reply);
};
