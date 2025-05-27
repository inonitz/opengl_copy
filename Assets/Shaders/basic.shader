#vertex shader
#version 460 core
layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
}

#fragment shader
#version 460 core

uniform vec3 u_col;

out vec4 frgCol;

void main()
{
	frgCol = vec4(u_col, 1.0f);
}