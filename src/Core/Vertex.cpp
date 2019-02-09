#include <Vertex.h>

Vertex::Vertex(QVector3D _position, QVector3D _normal, QVector3D _tangent, QVector3D _bitangent, QVector2D _texCoords) {
    position = _position;
    normal = _normal;
    tangent = _tangent;
    bitangent = _bitangent;
    texCoords = _texCoords;
}

Vertex::Vertex(const Vertex & other) {
    position = other.position;
    normal = other.normal;
    tangent = other.tangent;
    bitangent = other.bitangent;
    texCoords = other.texCoords;
}

Vertex & Vertex::operator=(const Vertex & other) {
    position = other.position;
    normal = other.normal;
    tangent = other.tangent;
    bitangent = other.bitangent;
    texCoords = other.texCoords;
    return *this;
}

bool Vertex::operator==(const Vertex & other) const {
    return position == other.position
        && normal == other.normal
        && tangent == other.tangent
        && bitangent == other.bitangent
        && texCoords == other.texCoords;
}

QDataStream &operator<<(QDataStream &out, const Vertex& vertex) {
    out << vertex.position;
    out << vertex.normal;
    out << vertex.tangent;
    out << vertex.bitangent;
    out << vertex.texCoords;
    return out;
}

QDataStream &operator>>(QDataStream &in, Vertex& vertex) {
    in >> vertex.position;
    in >> vertex.normal;
    in >> vertex.tangent;
    in >> vertex.bitangent;
    in >> vertex.texCoords;
    return in;
}
