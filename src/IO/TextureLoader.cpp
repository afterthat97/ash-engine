#include <IO/TextureLoader.h>
#include <QImageReader>

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

TextureLoader::TextureLoader() {}

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath, bool no_cache) {
#ifdef _DEBUG
    qDebug() << "TextureLoader::loadFromFile: loading" << filePath;
#endif
    if (no_cache || cache[filePath].isNull()) {
        QSharedPointer<Texture> texture(new Texture(textureType));
        QImageReader reader(filePath);
        texture->setObjectName(filePath);
        texture->setImage(reader.read().mirrored());
        if (texture->image().isNull()) {
#ifdef _DEBUG
            qDebug() << "Failed to load texture" << filePath << ":" << reader.errorString();
#endif
            m_log += "Failed to load texture " + filePath + ": " + reader.errorString() + '\n';
            return 0;
        }
        cache[filePath] = texture;
        return texture;
    }
#ifdef _DEBUG
    qDebug() << "TextureLoader::loadFromFile: texture reused";
#endif
    return cache[filePath];
}

bool TextureLoader::hasLog() { 
    return m_log != "";
}

QString TextureLoader::log() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}
