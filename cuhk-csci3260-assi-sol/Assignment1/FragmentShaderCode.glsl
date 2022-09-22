#version 430

layout(location = 0) out vec4 FragColor;
in vec4 theColor;

uniform vec4 u_Color;

void main()
{
	FragColor = theColor;
}