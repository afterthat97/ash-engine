#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;

out vec3 fragPos;
out vec2 fragTexCoords;
out mat3 TBN;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
    vec3 T = normalize(mat3(modelMat) * tangent);
    vec3 B = normalize(mat3(modelMat) * bitangent);
    vec3 N = normalize(transpose(inverse(mat3(modelMat))) * normal);

    gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0f);
    fragPos = vec3(modelMat * vec4(position, 1.0f));
    fragTexCoords = texCoords;
    TBN = mat3(T, B, N);
}
