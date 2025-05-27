#vertex shader
#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec3 tex;
uniform mat4 u_mvp;

out vec4 v_col;
out vec3 v_texPos;

void main()
{
	gl_Position = u_mvp * vec4(pos, 1.0);
	v_col = vec4(col, 1.0);
	v_texPos = tex;
}

#fragment shader
#version 450 core
in vec4 v_col;
in vece v_texPos;

uniform vec3 u_color;
uniform sampler3D u_texture;

out vec4 frag_col;

void main()
{
	frag_col = 0.5 * texture(u_texture, texPos) + 0.25 * (v_col + u_color);
}