#include <OpenGL/OpenGL.h>
#include <IO/IO.h>
#include <UI/UI.h>

void initApplication() {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

# ifdef WIN32
    QApplication::setFont(QFont("Microsoft YaHei UI", 9));
# endif

    QSurfaceFormat openGLFormat;
    openGLFormat.setVersion(3, 3);
    openGLFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(openGLFormat);
}

int main(int argc, char *argv[]) {
    initApplication();

    QApplication a(argc, argv);

    Scene* scene = new Scene;
    scene->addGridline(new Gridline);

    MainWindow* window = new MainWindow(scene, 0);
    window->show();

    return a.exec();
}
