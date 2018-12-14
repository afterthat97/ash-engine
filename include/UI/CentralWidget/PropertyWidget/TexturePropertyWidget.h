#pragma once

#include <Generic/Texture.h>
#include <UI/Common.h>

class TexturePropertyWidget: public QWidget {
    Q_OBJECT

public:
    TexturePropertyWidget(Texture* texture, QWidget *parent = Q_NULLPTR);
    ~TexturePropertyWidget();

private:
    Texture* texture;
    QCheckBox * enableTextureCheckBox;
    QLabel * typeLabel, *imageLabel;
    QVBoxLayout * mainLayout;

    void setupLayout();
    void setupSignals();

private slots:
    void setTextureEnabled(int state);
};