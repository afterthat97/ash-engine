#pragma once

#include <Texture.h>

class TexturePropertyWidget: public QWidget {
    Q_OBJECT

public:
    TexturePropertyWidget(Texture * texture, QWidget * parent = 0);

private:
    Texture* m_host;
    QCheckBox * m_enabledCheckBox;
    QLabel * m_textureTypeLabel, *m_imageLabel;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
};
