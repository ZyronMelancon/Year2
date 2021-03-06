#version 410
uniform vec3 kA = vec3(1,1,1);
uniform vec3 kD = vec3(1,1,1);
uniform vec3 kS = vec3(1,1,1);

uniform vec3 iA = vec3(0.25f,0.25f,0.25f);
uniform vec3 iD = vec3(1,1,1);
uniform vec3 iS = vec3(1,1,1);
uniform float iSpecPower = 32.0f;

in vec3 N;
in vec3 P;

uniform vec3 camPos = vec3(1,1,1);
uniform vec3 L = vec3(5,5,5);

in vec4 vColor;
out vec4 fragColor;
uniform vec4 zColor;
void main() 
{ 
vec3 Ambient = kA * iA;

float Ndl = max(0.0f, dot(N,-L));
vec3 Diffuse = kD * iD * Ndl;
vec3 R = reflect(L,N);
vec3 E = normalize(camPos-P);

float specTerm = pow(min(0.0f,dot(R,E)), iSpecPower);
vec3 Specular = kS * iS * specTerm;

fragColor = vec4(Ambient + Diffuse + Specular, 1) * vColor * zColor; 
}