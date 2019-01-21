#pragma once

#include <UI/Common.h>
#include <UI/SceneTreeWidget.h>
#include <OpenGL/OpenGLWidget.h>

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Scene * scene, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Scene *m_host;

    QMenuBar *m_menuBar;
    QWidget* m_centralWidget;
    QSplitter * m_splitter;
    QStatusBar * m_statusBar;
    QLabel* m_fpsLabel;

    SceneTreeWidget *m_sceneTreeWidget;
    OpenGLWindow *m_openGLWindow;
    QWidget *m_propertyWidget;

    QAction *actionFileNew, *actionFileOpen, *actionFileLoadModel, *actionFileSave, *actionFileExit;
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
    void itemDeselected(QVariant item);

    void fileNew();
    void fileOpen();
    void fileLoadModel();
    void fileSave();
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
