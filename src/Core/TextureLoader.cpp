#include <TextureLoader.h>

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath) {
    if (cache[filePath].isNull()) {
#ifdef DEBUG_OUTPUT
        dout << "Loading" << filePath;
#endif
        QSharedPointer<Texture> texture(new Texture(textureType));
        QImageReader reader(filePath);
        texture->setObjectName(filePath);
        texture->setImage(reader.read());

        if (texture->image().isNull()) {
            m_log += "Failed to load texture " + filePath + ": " + reader.errorString() + '\n';
#ifdef DEBUG_OUTPUT
            dout << "Failed to load texture:" << reader.errorString();
#endif
            return 0;
        }
        
        cache[filePath] = texture;
        return texture;
    }
#ifdef DEBUG_OUTPUT
    dout << filePath << "found in cache";
#endif
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
