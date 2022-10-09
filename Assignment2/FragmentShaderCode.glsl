#version 430

layout(location = 0) out vec4 FragColor;
in vec3 vertColor;
in vec2 v_TexCoord;

uniform sample2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	FragColor = vec4(vertColor, 1.0f);
}