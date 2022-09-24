#version 430

layout(location = 0) out vec4 FragColor;
in vec3 vertColor;

uniform vec4 u_Color;

void main()
{
	FragColor = vec4(vertColor, 1.0f);
}