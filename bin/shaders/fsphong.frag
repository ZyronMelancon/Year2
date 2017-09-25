#version 410
uniform vec3 kA = vec3(1,1,1);
uniform vec3 kD = vec3(1,1,1);
uniform vec3 kS = vec3(1,1,1);

uniform vec3 iA = vec3(0.1f,0.25f,0.25f);
uniform vec3 iD = vec3(1,1,1);
uniform vec3 iS = vec3(1,1,1);
uniform float iSpecPower;// = 50.0f;

in vec4 vColor;
in vec4 vNorm;
in vec4 vPosition;

uniform vec3 camPos;

uniform vec4 zColor;

out vec4 fragColor;


void main() 
{ 
vec3 Ambient = kA * iA;
vec3 L = normalize(vec3(-1,-1,0));
vec3 N = normalize(vNorm.xyz);
float Ndl = max(0.0f, dot(N,-L));


vec3 Diffuse = kD * iD * Ndl;


vec3 R = reflect(L,N);
vec3 E = normalize(camPos.xyz-vPosition.xyz);
float specTerm = pow(max(0, dot(R,E)), iSpecPower);


vec3 Specular = kS * iS * specTerm;

fragColor = vec4(Ambient + Diffuse + Specular, 1) * vColor * zColor ;
}