#version 330 core

in float objectColor;

out vec4 color;

void main() {
	color = vec4(objectColor);
} 
