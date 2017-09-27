#version 410
uniform vec3 kA = vec3(1,1,1);
uniform vec3 kD = vec3(1,1,1);
uniform vec3 kS = vec3(1,1,1);

uniform vec3 iA = vec3(.3f,.3f,.3f);
uniform vec3 iD = vec3(.7f,.7f,0.7f);
uniform vec3 iS = vec3(1,1,1);
uniform float iSpecPower;// = 50.0f;

in vec4 vColor;
in vec4 vNorm;
in vec4 vPosition;

uniform vec3 camPos;

out vec4 fragColor;


void main() 
{

vec3 Ambient = kA * iA;
vec3 L = normalize(vec3(0,-1,0));

vec3 N = normalize(vNorm.xyz);
float Ndl = max(0.0f, dot(N,-L));


vec3 Diffuse = kD * iD * Ndl;

vec3 E = normalize(camPos.xyz-vPosition.xyz);
vec3 H = normalize(E-L);
float specTerm = pow(max(0, dot(N,H)), iSpecPower);


vec3 Specular = kS * iS * specTerm;

fragColor = vec4( Ambient + Diffuse + Specular, 1) *vColor;

}