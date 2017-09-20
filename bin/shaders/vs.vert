#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
out vec4 vColor;
out vec4 vPosition;
uniform mat4 projectionViewWorldMatrix;
void main()
{
vPosition = position;
vColor = color;
gl_Position = projectionViewWorldMatrix * position;
}