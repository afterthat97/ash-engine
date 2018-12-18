#pragma once

#include <UI/MainMenuBar.h>
#include <UI/CentralWidget/CentralWidget.h>

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    friend class MainMenuBar;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    MainMenuBar * m_menuBar;
    CentralWidget * m_centralWidget;
    QStatusBar * m_statusBar;
};
