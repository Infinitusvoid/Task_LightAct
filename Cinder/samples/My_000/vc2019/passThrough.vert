#version 150 

uniform mat4 ciModelViewProjection;
uniform vec4 color_mult;

in vec4 ciPosition;
in vec4 ciColor;
in vec2 ciTexCoord0;

out vec4 vertColor;
out vec2 vertTexCoord0;
void main()
{
	vertColor = vec4(vertTexCoord0[0], 1.0, 0.0, 0.0);//ciColor;
	vertTexCoord0 = ciTexCoord0;
	gl_Position = ciModelViewProjection * ciPosition;
}
