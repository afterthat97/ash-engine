#include <UI/MainWindow.h>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef WIN32
    QApplication::setFont(QFont("Microsoft YaHei UI", 9));
#endif

    QApplication a(argc, argv);

    QSurfaceFormat openGLFormat;
    openGLFormat.setVersion(3, 3);
    openGLFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(openGLFormat);

    MainWindow w;
    w.resize(1000, 600);
    w.setWindowTitle("masterEngine");
    w.show();

    return a.exec();
}
