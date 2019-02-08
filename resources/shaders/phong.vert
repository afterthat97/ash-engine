layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;

out vec3 fragPos;
out vec2 fragTexCoords;
out mat3 TBN;

void main() {
    vec3 T = normalize(mat3(modelMat) * tangent);
    vec3 B = normalize(mat3(modelMat) * bitangent);
    vec3 N = normalize(mat3(normalMat) * normal);
    
    fragPos = vec3(modelMat * vec4(position, 1.0f));
    fragTexCoords = texCoords;
    TBN = mat3(T, B, N);

    mat4 MVP = projMat * viewMat * modelMat;
    gl_Position = MVP * vec4(position, 1.0f);
    if (sizeFixed == 1) {
        float w = (MVP * vec4(0.0f, 0.0f, 0.0f, 1.0f)).w / 100;
        gl_Position = MVP * vec4(position * w, 1.0f);
    }
}
