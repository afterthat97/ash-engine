#pragma once

#include <Camera.h>
#include <Vector3DEdit.h>
#include <FloatSlider.h>

class CameraProperty: public QWidget {
    Q_OBJECT

public:
    CameraProperty(Camera * camera, QWidget * parent = 0);

private:
    Camera *m_host;
    QLabel *m_movingSpeedLabel, *m_fieldOfViewLabel, *m_nearPlaneLabel, *m_farPlaneLabel;
    FloatEdit *m_movingSpeedEdit, *m_fieldOfViewEdit, *m_nearPlaneEdit, *m_farPlaneEdit;
    FloatSlider *m_movingSpeedSlider, *m_fieldOfViewSlider;
    Vector3DEdit *m_positionEdit, *m_directionEdit;
    QPushButton *m_resetButton;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};
