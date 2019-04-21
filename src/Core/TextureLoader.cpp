#include <TextureLoader.h>

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath) {
    if (cache[filePath].isNull()) {
        if (log_level >= LOG_LEVEL_INFO)
            dout << "Loading" << filePath;
        QSharedPointer<Texture> texture(new Texture(textureType));
        QImageReader reader(filePath);
        texture->setObjectName(filePath);
        texture->setImage(reader.read());

        if (texture->image().isNull()) {
            m_log += "Failed to load texture " + filePath + ": " + reader.errorString() + '\n';
            if (log_level >= LOG_LEVEL_ERROR)
                dout << "Failed to load texture:" << reader.errorString();
            return QSharedPointer<Texture>();
        }

        cache[filePath] = texture;
        return texture;
    }
    if (log_level >= LOG_LEVEL_INFO)
        dout << filePath << "found in cache";
    return cache[filePath];
}

bool TextureLoader::hasErrorLog() {
    return m_log != "";
}

QString TextureLoader::errorLog() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}
