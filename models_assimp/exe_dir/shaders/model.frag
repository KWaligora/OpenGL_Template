#version 330

uniform sampler2D texSampler;

in vec4 color; // kolor interpolowany z shadera wierzcholkow
in vec2 texCoord; // wspolrzedne tekstury

out vec4 fColor; // wyjsciowy kolor fragmentu
 
void main()
{
    vec4 texColor = texture(texSampler, texCoord);
    fColor = texColor;
}