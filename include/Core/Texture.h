#pragma once

#include <Core/Common.h>

class Texture: public QObject {
    Q_OBJECT

public:
    enum TextureType {
        Diffuse,
        Specular,
        Bump
    };
    Texture(TextureType textureType = Diffuse);
    Texture(const Texture& texture);

    void loadFromFile(QString filePath);
    void saveToFile();

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    bool enabled() const;
    TextureType textureType() const;
    QString filePath() const;
    const QImage & image() const;

public slots:
    void setEnabled(bool enabled);
    void setTextureType(TextureType textureType);

signals:
    void enabledChanged(bool enabled);
    void textureTypeChanged(TextureType textureType);
    void filePathChanged(const QString& filePath);
    void imageChanged(const QImage& image);

private:
    bool m_enabled;
    TextureType m_textureType;
    QString m_filePath;
    QImage m_image;
};
