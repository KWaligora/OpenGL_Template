#version 330
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec4 vPosition; // pozycja wierzcholka w lokalnym ukladzie wspolrzednych
layout (location = 2) in vec2 vTexCoord; //wspolrzedne tekstury

in vec4 vColor; // kolor wierzcholka
 
out vec4 color; // kolor przekazywany do shadera fragmentow
out vec2 texCoord; //wspolrzedne tekstury
 
void main()
{
    color = vColor;
    texCoord = vTexCoord;

    gl_Position = proj * view * model * vPosition;
}