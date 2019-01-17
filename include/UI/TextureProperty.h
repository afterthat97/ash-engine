#pragma once

#include <UI/Common.h>

class TexturePropertyWidget: public QWidget {
    Q_OBJECT

public:
    TexturePropertyWidget(Texture * texture, QWidget * parent = 0);
    // TODO: Copy constructor

private:
    Texture* m_host;
    QCheckBox * m_enabledCheckBox;
    QLabel * m_textureTypeLabel, *m_filePathLabel, *m_imageLabel;

    void configLayout();
    void configSignals();

private slots:
    // TODO: Connect to host's destroyed signal, commit suicide when host is destroyed.
};
