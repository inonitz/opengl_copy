#vertex shader
#version 450 core
layout(location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(pos, 1.0);
}

#fragment shader
#version 450 core

out vec4 col;

void main()
{
	col = vec4(1.0f, 0.0f, 1.0f, 1.0f);
};