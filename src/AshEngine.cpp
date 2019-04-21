#include <MainWindow.h>

int log_level = LOG_LEVEL_WARNING;

void initApplication() {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

#ifdef WIN32
    QApplication::setFont(QFont("Microsoft YaHei UI", 9));
#endif

    QSurfaceFormat openGLFormat;
    openGLFormat.setVersion(3, 3);
    openGLFormat.setProfile(QSurfaceFormat::CoreProfile);
    openGLFormat.setDepthBufferSize(24);
    openGLFormat.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(openGLFormat);
}

int main(int argc, char *argv[]) {
    initApplication();

    QApplication a(argc, argv);

    MainWindow *window = new MainWindow;
    window->show();

    return a.exec();
}
