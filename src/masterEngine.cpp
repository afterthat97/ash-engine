#include <OpenGL/OpenGL.h>
#include <IO/IO.h>
#include <UI/MainWindow.h>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
# ifdef WIN32
    QApplication::setFont(QFont("Microsoft YaHei UI", 9));
# endif
    QSurfaceFormat openGLFormat;
    openGLFormat.setVersion(3, 3);
    openGLFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(openGLFormat);

    QApplication a(argc, argv);

    Scene* scene = new Scene;
    scene->addGridline(new Gridline);
    scene->addLight(new DirectionalLight);

    //OpenGLWindow* window = new OpenGLWindow;
    //window->setRenderer(new OpenGLRenderer);
    //window->setScene(scene);
    //window->show();

    MainWindow* window = new MainWindow(scene, 0);
    window->resize(1080, 640);
    window->show();

    return a.exec();
}
