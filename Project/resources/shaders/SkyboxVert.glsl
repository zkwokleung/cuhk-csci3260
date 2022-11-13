#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_viewRotateMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
    TexCoords = aPos;
    gl_Position = u_projectionMatrix * u_viewRotateMatrix * vec4(aPos, 1.0);
}  