#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
layout(location=2) in vec4 normal;
out vec4 vColor;
out vec4 vPosition;
out vec4 vNorm;
uniform mat4 projectionViewWorldMatrix;
void main()
{
vPosition = position;
vNorm = normalize(position);
vColor = color;
gl_Position = projectionViewWorldMatrix * position;
}