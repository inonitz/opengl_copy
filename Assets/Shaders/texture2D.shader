#vertex shader
#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_texPos;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	v_texPos = vec2(texPos.x, texPos.y);
}

#fragment shader
#version 460 core
in vec2 v_texPos;

uniform float v_z;
uniform sampler2D u_texture;

out vec4 frag_col;

void main()
{
	float factor = 0.2;
	frag_col = texture(u_texture, v_texPos);
	frag_col /= v_z * factor;
}