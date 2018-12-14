#pragma once

#include <UI/Common.h>

class MainWindow;

class MainMenuBar: public QMenuBar {
    Q_OBJECT

public:
    MainMenuBar(MainWindow *parent);
    ~MainMenuBar();

private:
    MainWindow * mainWindow;
    QMenu *menuFile, *menuCreate, *menuCreateBasicShapes, *menuOptions, *menuHelp;
    QAction * actionFileNew, *actionFileOpen, *actionFileExit;
    QAction *actionCreateLight, *actionCreateBasicCube, *actionCreateBasicSphere, *actionCreateBasicPlane;
    QAction  *actionOptionEnableLighting, *actionOptionEnableGridline, *actionOptionEnableWireFrame;
    QAction *actionHelpAbout, *actionHelpCheckUpdate;

    void createActions();
    void createMenus();

private slots:
    void fileNew();
    void fileOpen();
    void fileExit();
    void createLight();
    void createBasicCube();
    void createBasicSphere();
    void createBasicPlane();
    void optionLighting(bool enabled);
    void optionGridline(bool enabled);
    void optionWireFrame(bool enabled);
    void helpAbout();
    void helpCheckForUpdates();
    void replyOfUpdates(QNetworkReply*);
};
