#version 330
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec4 vPosition; // pozycja wierzcholka w lokalnym ukladzie wspolrzednych

in vec4 vColor; // kolor wierzcholka
 
out vec4 color; // kolor przekazywany do shadera fragmentow
out vec2 texCoord; //wspolrzedne tekstury
 
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

    gl_Position = proj * view * model * vPosition;
}