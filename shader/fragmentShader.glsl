#version 330 core
out vec4 color;
in vec2 TexCoord;

uniform sampler2D texture0;

void main() {
	color = texture(texture0, TexCoord);
//	color = vec3(TexCoord, 0.0);
}
