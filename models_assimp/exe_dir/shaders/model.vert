#version 330
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix;

layout (location = 0) in vec4 vPosition; // pozycja wierzcholka w lokalnym ukladzie wspolrzednych
layout (location = 1) in vec3 vNormal;

in vec4 vColor; // kolor wierzcholka
 
out vec4 color; // kolor przekazywany do shadera fragmentow
out vec2 texCoord; //wspolrzedne tekstury

out vec3 normal;
out vec3 position;
 
 vec2 CalculateTexCoord()
 {
	float rho = sqrt(pow(vPosition.x,2) + pow(vPosition.y,2) + pow(vPosition.z,2));
	float theta = atan(vPosition.y/vPosition.x);
	float phi = acos(vPosition.z/rho);

	return vec2(theta, phi);
 }

void main()
{
    color = vColor;
    texCoord = CalculateTexCoord();

	normal = normalize(normalMatrix * vNormal);

	position = vec3( view * model * vPosition);

    gl_Position = proj * view * model * vPosition;
}