#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
layout(location=2) in vec4 normal;
layout(location=4) in vec2 uv;

out vec4 vColor;
out vec4 vPosition;
out vec4 vNorm;
out vec2 vUV;

uniform mat4 projectionViewWorldMatrix;
uniform mat4 model;

uniform sampler2D perlinTex;

void main()
{
vPosition = model * position;
vNorm = normal;
vColor = color;
vUV = uv;
vPosition.y += texture(perlinTex, vUV).r * 5;
gl_Position = projectionViewWorldMatrix * vPosition;
}