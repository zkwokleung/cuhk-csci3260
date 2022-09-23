#version 430

layout(location = 0) out vec4 FragColor;
in vec3 vertColor;

uniform vec4 u_Color;

void main()
{
	if(u_Color != 0)
	{
		FragColor = u_Color;
	}
	else
	{
		FragColor = vec4(vertColor, 1.0f);
	}
}