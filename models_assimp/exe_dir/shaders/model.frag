#version 330

uniform sampler2D texSampler;

// informacje o zrodle swiatla
uniform vec4 lightPosition; // we wspolrzednych oka
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// informacje o materiale
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

in vec4 color; // kolor interpolowany z shadera wierzcholkow
in vec2 texCoord; // wspolrzedne tekstury

in vec3 normal;
in vec3 position;

out vec4 fColor; // wyjsciowy kolor fragmentu
 
void main()
{
    vec4 texColor = texture(texSampler, texCoord);

	vec3 norm = normalize( normal );
	vec3 lightDir = normalize( vec3( lightPosition ) - position );
	vec3 viewDir = normalize( vec3( 0.0, 0.0, 0.0 ) - position );

	vec3 ambient = lightAmbient * materialAmbient;

	vec3 diffuse = lightDiffuse * materialDiffuse * max( dot( lightDir, norm ), 0.0 );

	vec3 specular = vec3( 0.0, 0.0, 0.0 );
	if( dot( lightDir, viewDir ) > 0.0 )
	{
		vec3 refl = reflect( vec3( 0.0, 0.0, 0.0 ) - lightDir, norm );
		specular = pow( max( 0.0, dot( viewDir, refl ) ), materialShininess ) * materialSpecular * lightSpecular;
	}

    fColor = vec4( clamp( ambient + diffuse + specular, 0.0, 1.0 ), 1.0 ) * texColor;
}