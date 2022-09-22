#version 430

in layout(location=0) vec3 position;
in layout (location = 1) vec3 vertexColor;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec3 elColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * v;
	elColor = vertexColor;
}
