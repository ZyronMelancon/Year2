#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
out vec4 vColor;
uniform float time;
uniform mat4 projectionViewWorldMatrix;
void main()
{
vColor = color;
vec4 disp = vec4(cos(time), sin(time), cos(time)/sin(time), 1);
vec4 np = disp + position;
gl_Position = projectionViewWorldMatrix * np;
}