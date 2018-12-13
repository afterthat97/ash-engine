#pragma once

#include <Generic/Texture.h>
#include <UI/Vector3DWidget.h>

class TexturePropertyWidget: public QWidget {
    Q_OBJECT

public:
    TexturePropertyWidget(Texture* texture, QWidget *parent = Q_NULLPTR);
    ~TexturePropertyWidget();

private:
    Texture* texture;
    QCheckBox * enableTextureCheckBox;
    QLabel * typeLabel, * imageLabel;

    void setupLayout();
    void setupSignals();

private slots:
    void setTextureEnabled(int state);
};