#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec2 uv;
in layout(location=2) vec3 normal;

// MVP
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

// Position, rotation, scale
uniform mat4 u_positionMat4;
uniform mat4 u_rotationMat4;
uniform mat4 u_scaleMat4;

// Output to fragment shader
out vec3 v_position;
out vec2 v_uv;
out vec3 v_normal;

void main()
{
	// Position
	vec4 v = vec4(position, 1.0);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * v;
	vec4 n = vec4(normal, 1.0);

	// UV and lighting
	v_position = vec3(u_modelMatrix * v);
	v_uv = uv;
	v_normal = normalize(vec3(u_rotationMat4 * n));
}
