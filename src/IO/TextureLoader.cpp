#include <IO/TextureLoader.h>
#include <QImageReader>

// TODO: Debug info

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

TextureLoader::TextureLoader() {}

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath, bool no_cache) {
    if (no_cache || cache[filePath].isNull()) {
        QSharedPointer<Texture> texture(new Texture(textureType));
        qDebug() << "Loading texture" << filePath;
        QImageReader reader(filePath);
        texture->setImage(reader.read().mirrored());
        if (texture->image().isNull()) {
            qDebug() << "Failed to load texture:" << reader.errorString();
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
