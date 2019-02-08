out vec4 fragColor;

void main() {
    fragColor = vec4(vec3(material.color), 1.0f);
    
    if (highlighted == 1)
        fragColor += vec4(0.2, 0.2, 0.2, 0);
    
    if (selected == 1)
        fragColor += vec4(0, 0, 0.4, 0);
}
