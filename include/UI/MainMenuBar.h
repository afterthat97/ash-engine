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
    QMenu *menuFile, *menuCreate, *menuCreateBasicShapes, *menuOptions, *menuSetFPS, *menuHelp;
    QAction * actionFileNew, *actionFileOpen, *actionFileExit;
    QAction *actionCreateLight, *actionCreateBasicCube, *actionCreateBasicSphere, *actionCreateBasicPlane;
    QAction *actionEnableLighting, *actionEnableGridline, *actionEnableWireFrame, *actionSetFPS30, *actionSetFPS60;
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
    void setFPS30();
    void setFPS60();
    void helpAbout();
    void helpCheckForUpdates();
    void replyOfUpdates(QNetworkReply*);
};
