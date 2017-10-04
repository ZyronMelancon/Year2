#version 410

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D perlinTex;

void main() 
{
fragColor = texture(perlinTex, vUV).rrrr;
fragColor.a = 1;
}