#pragma once
#include "bhdr.h"
#include "Window.h"

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

	vec3 m_forward {0.0f, 0.0f, -1.0f};
	vec3 m_up      {0.0f};
	vec3 m_right   {0.0f};
public:
	vec3 position;
	vec3 world_up;
	mat4 CameraTransform; // view matrix
	mat4 ProjectionMatrix; // projection matrix.

	CameraFPS(Window& win): 
		win_handle(&win),

		position(0.0f, 0.0f, 3.0f),
		world_up(0.0f, 1.0f, 0.0f),

		m_firstPos(win.Width() / 2, win.Height() / 2),
		m_pitch(0.0f),
		m_yaw(0.0f),
		m_vel(2.5f),

		Near(0.1f),
		Far(100.0f),
		fov(rad(45))
	{
		updateCameraAxes();
		glfwSetKeyCallback(win_handle->getHandle(), key_callback);
	}



	CameraFPS(
		Window& win,

		vec3&& position,
		vec3&& up,

		const float& pitch,
		const float& yaw,
		const float& vel,

		const float& Near,
		const float& Far,
		const float& fov
	) :
		win_handle(&win),

		position(std::move(position)),
		world_up(std::move(up)),

		m_firstPos(win.Width() / 2, win.Height() / 2),
		m_pitch(pitch),
		m_yaw(yaw),
		m_vel(vel),

		Near(Near),
		Far(Far),
		fov(fov)
	{
		updateCameraAxes();
		glfwSetKeyCallback(win_handle->getHandle(), key_callback);
	}


	CameraFPS(
		Window& win,

		vec3& position,
		vec3& up,

		const float& pitch,
		const float& yaw,
		const float& vel,

		const float& Near,
		const float& Far,
		const float& fov
	) :
		win_handle(&win),

		position(position),
		world_up(up),

		m_firstPos(win.Width() / 2, win.Height() / 2),
		m_pitch(pitch),
		m_yaw(yaw),
		m_vel(vel),

		Near(Near),
		Far(Far),
		fov(fov)
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

		m_pitch = clamp(m_pitch, -89.9f, 89.9f);
		m_yaw   = clamp(m_yaw  , -74.9f, 74.9f);
		updateCameraAxes();

		vec3 tmp{ m_vel * win_handle->dt };
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

		add(&position, &m_forward, &tmp);
		lookAt(position, tmp, m_up, &CameraTransform);
		perspective(Near, Far, win_handle->Width() / win_handle->Height(), fov, &ProjectionMatrix);
	}

	mat4* getView()
	{
		return &CameraTransform;
	}

	mat4* getProjection()
	{
		return &ProjectionMatrix;
	}
private:
	void updateCameraAxes()
	{
		m_forward.set(
			fcos(m_yaw) * fcos(m_pitch),
			fsin(m_pitch),
			fsin(m_yaw) * fcos(m_pitch)
		); 
		m_forward = m_forward.normalize();

		m_forward.cross(world_up, &m_right);
		m_right = m_right.normalize();

		m_right.cross(m_forward, &m_up);
		m_up = m_up.normalize();
	}
};