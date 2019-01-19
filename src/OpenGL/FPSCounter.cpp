#include <OpenGL/FPSCounter.h>

FPSCounter::FPSCounter(QObject* parent): QObject(parent) {
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1000);
}

void FPSCounter::inc() {
    fps++;
}

void FPSCounter::timeout() {
    fpsChanged(fps);
    fps = 0;
}