#version 430

layout(location = 0) out vec3 FragColor;

in vec3 v_position;
in vec2 v_uv;
in vec3 v_normal;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, v_uv).rgb;
}