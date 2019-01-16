#include <OpenGL/OpenGL.h>
#include <IO/IO.h>
#include <UI/MainWindow.h>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
# ifdef WIN32
    QApplication::setFont(QFont("Microsoft YaHei UI", 9));
# endif
    QApplication a(argc, argv);

    Scene* scene = new Scene;
    scene->addGridline(new Gridline);
    scene->addLight(new DirectionalLight);

    MainWindow* window = new MainWindow(scene, 0);
    window->resize(1080, 640);
    window->show();

    return a.exec();
}
