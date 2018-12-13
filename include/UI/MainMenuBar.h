#pragma once

#include <UI/Common.h>

class MainMenuBar: public QMenuBar {
    Q_OBJECT

public:
    MainMenuBar(QWidget *parent = Q_NULLPTR);
    ~MainMenuBar();

private:
    QMenu *menuFile, *menuCreate, *menuCreateBasicShapes, *menuOptions, *menuHelp;
    QAction * actionFileNew, *actionFileOpen, *actionFileExit;
    QAction *actionCreateLight, *actionCreateBasicCube, *actionCreateBasicSphere, *actionCreateBasicPlane;
    QAction  *actionOptionEnableLighting, *actionOptionEnableGridline, *actionOptionEnableWireFrame;
    QAction *actionHelpAbout, *actionHelpCheckUpdate;

    void createActions();
    void createMenus();

signals:
    void sceneChanged();

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
