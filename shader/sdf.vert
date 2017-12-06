#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float color;

uniform mat4 PVM;

out float objectColor;

void main() {
	objectColor = color;
	gl_Position = PVM * vec4(aPos, 1.0f);
}
