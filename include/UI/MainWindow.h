#pragma once

#include <OpenGLWindow.h>
#include <SceneTreeWidget.h>

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Scene *m_host;
    QString m_sceneFilePath;
    QVariant m_copyedObject;

    QSplitter * m_splitter;
    QLabel* m_fpsLabel;

    SceneTreeWidget *m_sceneTreeWidget;
    OpenGLWindow *m_openGLWindow;
    QScrollArea *m_propertyWidget;

    bool askToSaveScene();

    void configMenu();
    void configLayout();
    void configSignals();

private slots:
    void fpsChanged(int fps);
    void itemSelected(QVariant item);
    void itemDeselected(QVariant item);

    void fileNewScene();
    void fileOpenScene();
    void fileImportModel();
    void fileExportModel();
    void fileSaveScene();
    void fileSaveAsScene();
    void fileQuit();

    void editCopy();
    void editPaste();
    void editRemove();

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

    void polygonAssignMaterial();
    void polygonReverseNormals();
    void polygonReverseTangents();
    void polygonReverseBitangents();

    void gizmoAlwaysOnTop(bool alwaysOnTop);
    void gizmoTypeTranslate();
    void gizmoTypeRotate();
    void gizmoTypeScale();

    void helpCheckForUpdates();
    void helpSourceCode();
    void helpBugReport();
    void helpFeatureRequest();
    void helpSystemInfo();
    void helpAbout();

    void replyOfUpdates(QNetworkReply* reply);
};
