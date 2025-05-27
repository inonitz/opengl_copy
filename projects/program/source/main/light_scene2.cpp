#include "Shader.h"
#include "Buffers.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static Array<float> verts_cube =
{
	 // pos              // tex       // normal   
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0,
};


static Array<float> verts_light =
{
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};




int light_scene2()
{

	Window window(1080, 720, true);
	window.setClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });

	CameraFPS cam_(window);

	glm::mat4x4
		cube_model,
		transform,
		light_model,
		tmp,
		tmp2;
	glm::vec3
		angle_axis{ 0.5f, 1.0f, 0.0f },
		trs{0.0f},
		light_col{1.0f},
		light_pos{-1.0f, 2.0f, -2.0f};

	Shader light_shader("Assets/Shaders/basic.shader"); light_shader.link();
	Array<Vertex> lattribs =
	{
		{vtype::vec3, dtype::Float, true}
	};
	VBO light_buffer(verts_light, lattribs, USAGE::STATIC);


	Shader cube_shader("Assets/Shaders/basic_lighting.shader"); cube_shader.link();
	Array<Vertex> attribs =
	{
		Vertex(vtype::vec3, dtype::Float, true), // pos
		Vertex(vtype::vec2, dtype::Float, true), // tex pos
		Vertex(vtype::vec3, dtype::Float, true)  // normal
	};
	VBO cube_buffer(verts_cube, attribs, USAGE::STATIC);
	Texture2D tex0("Assets/default.jpg", { texarg::MIPMAP_NEAREST_GL_NEAREST, texarg::MIPMAP_NEAREST_GL_NEAREST, texarg::REPEAT, texarg::REPEAT },  true);



	light_model = glm::identity<glm::mat4x4>();
	light_model = glm::translate(light_model, light_pos);
	light_model = glm::scale(light_model, glm::vec3{0.5f});

	cube_model = glm::identity<glm::mat4x4>();
	cube_model = glm::rotate(cube_model, 0.0f, angle_axis);

	while (!window.shouldClose())
	{
		// reset the scene
		window.update();
		window.clearScreen();
		cam_.update();

		// calculate normal matrix
		tmp2 = cam_.getView() * cube_model;
		tmp  = glm::inverse(tmp2);
		tmp  = glm::transpose(tmp);

		// draw textured cube 1
		cube_shader.use();
		cube_buffer.bindVAO();
		cube_shader.setUniform1i("u_texture", 0);

		cube_shader.setUniformMat4("model",      glm::value_ptr(cube_model));
		cube_shader.setUniformMat4("view",       glm::value_ptr(cam_.getView()      ));
		cube_shader.setUniformMat4("projection", glm::value_ptr(cam_.getProjection()));
		cube_shader.setUniformMat4("transform_nrml", glm::value_ptr(tmp));

		cube_shader.setUniform3f("Li_c", glm::value_ptr(light_col));
		cube_shader.setUniform3f("Li_p", glm::value_ptr(light_pos));
		cube_shader.setUniform3f("cam_p", glm::value_ptr(cam_.position));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// draw "light" cube 2
		light_shader.use();
		light_buffer.bindVAO();
		light_shader.setUniformMat4("model",	  glm::value_ptr(light_model));
		light_shader.setUniformMat4("view",       glm::value_ptr(cam_.getView()      ));
		light_shader.setUniformMat4("projection", glm::value_ptr(cam_.getProjection()));
		light_shader.setUniform3f("u_col", glm::value_ptr(light_col));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// update 
		tmp2 = glm::translate(glm::identity<glm::mat4x4>(), trs);
		tmp  = glm::rotate(cube_model, window.timeElapsed, angle_axis);
		cube_model = tmp2 * tmp;
	}
};


/*
	use angle-axis rotation matrix to rotate the camera based on mouse movement.
	the movement of the mouse dictates the axis && angle in which the camera is rotating.
*/


