#pragma once

#include <Material.h>
#include <Vector3DEditSlider.h>

class TextureProperty;

class MaterialProperty: public QWidget {
    Q_OBJECT

public:
    MaterialProperty(Material* material, QWidget *parent = 0);

private:
    Material *m_host;
    QLabel *m_ambientLabel, *m_diffuseLabel, *m_specularLabel, *m_shininessLabel;
    FloatEdit *m_ambientEdit, *m_diffuseEdit, *m_specularEdit, *m_shininessEdit;
    FloatSlider *m_ambientSlider, *m_diffuseSlider, *m_specularSlider, *m_shininessSlider;
    Vector3DEditSlider *m_colorEditSlider;
    TextureProperty *m_diffuseMapProperty, *m_specularMapProperty, *m_bumpMapProperty;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject * host);
};

class TextureProperty: public QGroupBox {
    Q_OBJECT

public:
    TextureProperty(Texture * texture, QWidget * parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Texture* m_host;
    QLabel *m_widthTextLabel, *m_widthValueLabel;
    QLabel *m_heightTextLabel, *m_heightValueLabel;
    QLabel *m_imageLabel;

    void configLayout();
    void configSignals();

private slots:
    void hostDestroyed(QObject* host);
    void textureTypeChanged(Texture::TextureType textureType);
    void imageChanged(const QImage& image);
};