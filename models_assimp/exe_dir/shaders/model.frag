#version 330

uniform sampler2D texSampler;

// informacje o zrodle swiatla
uniform vec4 lightPosition; // we wspolrzednych oka
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

in vec4 color; // kolor interpolowany z shadera wierzcholkow
in vec2 texCoord; // wspolrzedne tekstury

out vec4 fColor; // wyjsciowy kolor fragmentu
 
void main()
{
//    vec4 texColor = texture(texSampler, texCoord);
//    fColor = texColor;

	fColor = vec4(lightDiffuse, 1.0f);
}