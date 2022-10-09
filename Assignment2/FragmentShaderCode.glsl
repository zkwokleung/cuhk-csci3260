#version 430

layout(location = 0) out vec4 FragColor;
in vec2 v_uv;

uniform sample2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, v_uv).rgb;
}