#include <IO/TextureLoader.h>

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

TextureLoader::TextureLoader() {}

TextureLoader::~TextureLoader() {}

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath, bool no_cache) {
    if (no_cache || cache[filePath].isNull()) {
        QSharedPointer<Texture> texture(new Texture);
        texture->setTextureType(textureType);
        qDebug() << "Loading texture" << filePath;
        texture->loadFromFile(filePath);
        if (texture->image().isNull()) {
            qDebug() << "Failed to load texture" << filePath;
            m_log += "Failed to load texture " + filePath + ".\n";
            return 0;
        }
        cache[filePath] = texture;
        return texture;
    }
    return cache[filePath];
}

bool TextureLoader::hasLog() { 
    return m_log.length() != 0;
}

QString TextureLoader::log() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}