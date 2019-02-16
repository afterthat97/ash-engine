#pragma once

#include <Texture.h>

class TextureLoader {
public:
    TextureLoader() {}

    QSharedPointer<Texture> loadFromFile(Texture::TextureType textureType, QString filePath);

    bool hasErrorLog();
    QString errorLog();

private:
    QString m_log;
    static QHash<QString, QWeakPointer<Texture> > cache;
};
