#pragma once

#include <OpenGL/OpenGLWidget.h>
#include <UI/SceneTreeView.h>
#include <UI/MainMenuBar.h>

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    MainMenuBar * menuBar;
    QWidget * centralWidget;
    QSplitter * splitter;
    SceneTreeView * sceneTreeView;
    OpenGLWidget * openGLWidget;
    QWidget * propertyWidget;
    QStatusBar * statusBar;

    QWidget * createEmptyPropertyWidget();
    QWidget * createCentralWidget();
    QStatusBar * createStatusBar();

private slots:
    void resetUI();
    void reloadPropertyWidget(const QModelIndex & current, const QModelIndex & previous);
};
