#version 410
in vec4 vColor;
in vec2 vUV;
out vec4 fragColor;

uniform sampler2D tex;

void main() { fragColor = texture(tex, vUV);}