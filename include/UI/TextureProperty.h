#pragma once

#include <UI/Common.h>

class TexturePropertyWidget: public QWidget {
    Q_OBJECT

public:
    TexturePropertyWidget(Texture * texture, QWidget * parent = 0);

private:
    Texture* m_host;
    QCheckBox * m_enabledCheckBox;
    QLabel * m_textureTypeLabel, *m_filePathLabel, *m_imageLabel;

    void configLayout();
    void configSignals();
};
