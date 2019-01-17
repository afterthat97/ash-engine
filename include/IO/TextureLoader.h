#pragma once

#include <Core/Core.h>

class TextureLoader {
public:
    TextureLoader();

    QSharedPointer<Texture> loadFromFile(Texture::TextureType textureType, QString filePath, bool no_cache = false);

    bool hasLog();
    QString log();

private:
    QString m_log;
    static QHash<QString, QWeakPointer<Texture>> cache;
};
