#pragma once

#include <UI/MainMenuBar.h>
#include <UI/CentralWidget/CentralWidget.h>

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    friend class MainMenuBar;
    friend class SceneTreeView;
    friend class OpenGLWidget;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void modelSelected(Model* model, bool selected);
    void lightSelected(Light* light, bool selected);
    void meshSelected(Mesh* mesh, bool selected);
    void materialSelected(Material* material, bool selected);
    void textureSelected(Texture* texture, bool selected);

private:
    MainMenuBar * menuBar;
    CentralWidget * centralWidget;
    QStatusBar * statusBar;
};
