#vertex shader
#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

uniform mat4 u_transform;

out vec4 v_col;

void main()
{
	gl_Position = u_transform * vec4(pos, 1.0);
	v_col = vec4(col, 1.0);
}

#fragment shader
#version 450 core
in vec4 v_col;
uniform vec3 u_col;

out vec4 frgCol;

void main()
{
	frgCol = 1.0 * (vec4(u_col, 1.0) + v_col);
}