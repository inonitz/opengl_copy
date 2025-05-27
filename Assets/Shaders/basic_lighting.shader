#vertex shader
#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform_nrml;

out vec2 v_texPos;
out vec3 v_normal;
out vec4 v_frag_p;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	v_texPos = tex;
	v_normal = mat3(transform_nrml) * normal;
	v_frag_p = model * vec4(pos, 1.0f);
}

#fragment shader
#version 460 core
in vec2 v_texPos;
in vec3 v_normal;
in vec4 v_frag_p;

float ambience = 0.3f;
float specularStrength = 0.5;
float shininess_pow = 64;

uniform sampler2D u_texture;
uniform vec3 Li_c;
uniform vec3 Li_p;
uniform vec3 cam_p;

out vec4 frag_col;

void main()
{
	vec3 light_dir = normalize(Li_p - v_frag_p.xyz);
	vec3 normal = normalize(v_normal);
	float diffuse = max( dot(normal, light_dir), 0 );
	float specular = specularStrength * 
	pow(
		max(
			dot(normalize(cam_p - v_frag_p.xyz),
				reflect(-light_dir, normal))
			,0.0f)
		,shininess_pow
	);

	frag_col = texture(u_texture, v_texPos) * vec4(Li_c, 1.0f) * (ambience + diffuse + specular);
}