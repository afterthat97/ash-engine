#include <OpenGL/OpenGL.h>
#include <IO/IO.h>
#include <UI/UI.h>

void initApplication() {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

#ifdef WIN32
    QApplication::setFont(QFont("Microsoft YaHei UI", 9));
#endif

    QSurfaceFormat openGLFormat;
    openGLFormat.setVersion(4, 2);
    openGLFormat.setProfile(QSurfaceFormat::CoreProfile);
    openGLFormat.setDepthBufferSize(24);
    openGLFormat.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(openGLFormat);
}

int main(int argc, char *argv[]) {
    initApplication();

    QApplication a(argc, argv);

    Scene* scene = new Scene;
    scene->addGridline(new Gridline);
    scene->addDirectionalLight(new DirectionalLight);
    scene->addModel(ModelLoader::loadCubeModel());

    MainWindow *window = new MainWindow(scene);
    window->show();

    return a.exec();
}
