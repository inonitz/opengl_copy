#pragma once
#include "Window.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


enum class MOVEMENT_KEY 
{ 
	NONE = GLFW_KEY_UNKNOWN,
	W	 = GLFW_KEY_W,
	S	 = GLFW_KEY_S,
	A	 = GLFW_KEY_A,
	D	 = GLFW_KEY_D,
	R	 = GLFW_KEY_R,
	F	 = GLFW_KEY_F
};
#define MOUSE_SENSITIVITY 0.01
inline MOVEMENT_KEY CURRENT_KEYSTROKE = MOVEMENT_KEY::NONE;
inline bool PLAYER_NOT_EXIT = true;
struct bit_ {
	uint8_t m_b : 1;
	bit_(const bool& b) : m_b(b) {}
	bit_& operator=(const bool& b)
	{
		m_b = b;
	}
};

inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// when pressed, action wont affect the key value
	// therefore, the GLFW_KEY value would still be valid.
	uint32_t condition = key * (action == GLFW_PRESS || action == GLFW_REPEAT);
	switch (condition)
	{
	case GLFW_RELEASE:    CURRENT_KEYSTROKE = MOVEMENT_KEY::NONE; break;
	case GLFW_KEY_W:	  CURRENT_KEYSTROKE = MOVEMENT_KEY::W;	  break;
	case GLFW_KEY_S:	  CURRENT_KEYSTROKE = MOVEMENT_KEY::S;	  break;
	case GLFW_KEY_A:	  CURRENT_KEYSTROKE = MOVEMENT_KEY::A;	  break;
	case GLFW_KEY_D:	  CURRENT_KEYSTROKE = MOVEMENT_KEY::D;	  break;
	case GLFW_KEY_R:	  CURRENT_KEYSTROKE = MOVEMENT_KEY::R;	  break;
	case GLFW_KEY_F:	  CURRENT_KEYSTROKE = MOVEMENT_KEY::F;    break;
	case GLFW_KEY_ESCAPE: PLAYER_NOT_EXIT = !PLAYER_NOT_EXIT;	  break;
	}
}


class CameraFPS
{
private:
	Window* win_handle;

	MouseCoords m_firstPos;
	float m_pitch;
	float m_yaw;
	float m_vel;

	float Near;
	float Far;
	float fov;

	glm::vec3 m_forward {0.0f, 0.0f, -1.0f};
	glm::vec3 m_up      {0.0f};
	glm::vec3 m_right   {0.0f};
public:
	glm::vec3 position;
	glm::vec3 world_up;
	glm::mat4x4 CameraTransform; // view matrix
	glm::mat4x4 ProjectionMatrix; // projection matrix.

	CameraFPS(Window& win): 
		win_handle(&win),

		m_firstPos(win.Width() / 2, win.Height() / 2),
		m_pitch(0.0f),

		m_yaw(0.0f),
		m_vel(2.5f),
		Near(0.1f),
		Far(100.0f),

		fov(glm::radians(45.0f)),
		position(0.0f, 0.0f, 3.0f),
		world_up(0.0f, 1.0f, 0.0f)
	{
		updateCameraAxes();
		glfwSetKeyCallback(win_handle->getHandle(), key_callback);
	}



	CameraFPS(
		Window& win,

		glm::vec3&& position,
		glm::vec3&& up,

		const float& pitch,
		const float& yaw,
		const float& vel,

		const float& Near,
		const float& Far,
		const float& fov
	) :
		win_handle(&win),

		m_firstPos(win.Width() / 2, win.Height() / 2),
		m_pitch(pitch),

		m_yaw(yaw),
		m_vel(vel),
		Near(Near),
		Far(Far),

		fov(fov),
		position(std::move(position)),
		world_up(std::move(up))
	{
		updateCameraAxes();
		glfwSetKeyCallback(win_handle->getHandle(), key_callback);
	}


	CameraFPS(
		Window& win,

		glm::vec3& position,
		glm::vec3& up,

		const float& pitch,
		const float& yaw,
		const float& vel,

		const float& Near,
		const float& Far,
		const float& fov
	) :
		win_handle(&win),

		m_firstPos(win.Width() / 2, win.Height() / 2),
		m_pitch(pitch),

		m_yaw(yaw),
		m_vel(vel),
		Near(Near),
		Far(Far),

		fov(fov),
		position(position),
		world_up(up)
	{
		updateCameraAxes();
		glfwSetKeyCallback(win_handle->getHandle(), key_callback);
	}

	void update()
	{
		MouseCoords* last = win_handle->getCursor();
		m_pitch += static_cast<float>((last->y - m_firstPos.y) * MOUSE_SENSITIVITY) * PLAYER_NOT_EXIT;
		m_yaw   += static_cast<float>((last->x - m_firstPos.x) * MOUSE_SENSITIVITY) * PLAYER_NOT_EXIT;
		m_firstPos = *last;

		m_pitch = glm::clamp(m_pitch, -89.9f, 89.9f);
		m_yaw   = glm::clamp(m_yaw  , -74.9f, 74.9f);
		updateCameraAxes();

		glm::vec3 tmp{ m_vel * win_handle->dt };
		switch (CURRENT_KEYSTROKE)
		{
		case MOVEMENT_KEY::NONE:								 break;
		case MOVEMENT_KEY::W: tmp *= m_forward; position += tmp; break;
		case MOVEMENT_KEY::S: tmp *= m_forward; position -= tmp; break;
		case MOVEMENT_KEY::D: tmp *= m_right  ; position += tmp; break;
		case MOVEMENT_KEY::A: tmp *= m_right  ; position -= tmp; break;
		case MOVEMENT_KEY::R: tmp *= m_up     ; position += tmp; break;
		case MOVEMENT_KEY::F: tmp *= m_up	  ; position -= tmp; break;
		}


		tmp = position + m_forward;
		CameraTransform  = glm::lookAt(position, tmp, m_up);
		ProjectionMatrix = glm::perspective(fov, win_handle->Width() / win_handle->Height(), Near, Far);
	}

	glm::mat4x4& getView()
	{
		return CameraTransform;
	}

	glm::mat4x4& getProjection()
	{
		return ProjectionMatrix;
	}
private:
	void updateCameraAxes()
	{
		m_forward = glm::normalize(glm::vec3{
			glm::cos(m_yaw) * glm::cos(m_pitch),
			glm::sin(m_pitch),
			glm::sin(m_yaw) * glm::cos(m_pitch)
		});

		m_right = glm::normalize(glm::cross(m_forward, world_up));
		m_up 	= glm::normalize(glm::cross(m_right, m_forward));
		return;
	}
};