#version 410
in vec4 vColor;
out vec4 fragColor;
uniform vec4 zColor;
void main() { fragColor = vColor * zColor; }