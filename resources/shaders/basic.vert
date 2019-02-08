layout (location = 0) in vec3 position;

void main() {
    mat4 MVP = projMat * viewMat * modelMat;
    gl_Position = MVP * vec4(position, 1.0f);
    if (sizeFixed == 1) {
        float w = (MVP * vec4(0.0f, 0.0f, 0.0f, 1.0f)).w / 100;
        gl_Position = MVP * vec4(position * w, 1.0f);
    }
}
