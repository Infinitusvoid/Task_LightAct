#version 150

uniform sampler2D iSrc;
uniform sampler2D iDst;
uniform float     iFade;

uniform vec4 color_mult;

in vec4 vertColor;
in vec2 vertTexCoord0;

out vec4 fragColor;

void main()
{
	vec4 srcColor = texture( iSrc, vertTexCoord0 );
	
	if(color_mult[3] == 1.0)
	{
		fragColor = srcColor * vec4(vec3(color_mult), 1.0);
	}
	else
	{
		fragColor = vec4(srcColor.rrr, 1.0);
	}
	
}