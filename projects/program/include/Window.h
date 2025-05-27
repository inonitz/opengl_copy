#pragma once
#include <Windows.h>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ResourceManager.h"


inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

struct MouseCoords
{
	double x, y;
	MouseCoords() : x(0.0f), y(0.0f) {}
	MouseCoords(
		double x, 
		double y
	) : x(x), y(y) {}
};


class Window
{
private:
	GLFWwindow* m_handle;
	int32_t m_height;
	int32_t m_width;

	glm::vec4 ScreenColor{ 0.7f, 0.7f, 0.7f, 1.0f };
public:
	MouseCoords m_mousePos;
	float timeElapsed;
	float frameCount;
	float t0, t1, dt;
	Window(const int32_t& w, const int32_t& h, bool GL_CORE = false) : m_height(h), m_width(w), timeElapsed(0.0f), frameCount(0.0f)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		if (GL_CORE) glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		else         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		GLFWwindow* window = glfwCreateWindow(w, h, "fuck you", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			ERR_EXIT("Failed to Create GLFW window");
		}
		t0 = (float)glfwGetTime();

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetKeyCallback(m_handle, key_callback); // <-- for overloaded key_callback function [top of header]
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			ERR_EXIT("Failed to initialize GLAD");
		}

		t1 = (float)glfwGetTime();
		dt = t1 - t0;
		t0 = t1;
		m_handle = window;
		glfwSetCursorPos(m_handle, m_width / 2, m_height / 2);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glEnable(GL_DEPTH_TEST);
	}

	inline MouseCoords* getCursor() 
	{ 
		glfwGetCursorPos(m_handle, &m_mousePos.x, &m_mousePos.y); 
		return &m_mousePos;
	}
	inline void setCursorPos(const float& x, const float& y)
	{
		glfwSetCursorPos(m_handle, x, y);
	}

	inline void   setClearColor(glm::vec4 col)
	{
		ScreenColor = col;
		glClearColor(ScreenColor.x, ScreenColor.y, ScreenColor.z, ScreenColor.w);
	}
	inline void   setClearColor(glm::vec3 col)
	{
		ScreenColor = glm::vec4{col, 1.0f};
		glClearColor(ScreenColor.x, ScreenColor.y, ScreenColor.z, ScreenColor.w);
	}

	inline float  deltaTime() { return dt; }
	inline bool   keyPressed(const unsigned int& K) { return glfwGetKey(m_handle, K) == GLFW_PRESS; }
	inline bool   shouldClose() { return glfwWindowShouldClose(m_handle); }
	inline float  Width()  { return (float)m_width;  }
	inline float  Height() { return (float)m_height; }
	inline GLFWwindow* getHandle() { return m_handle;  }
	inline void   update()
	{
		t1 = (float)glfwGetTime();
		dt = t1 - t0;
		t0 = t1;
		timeElapsed += dt;
		++frameCount;
		glfwGetWindowSize(m_handle, &m_width, &m_height);
		glfwGetCursorPos(
			m_handle,
			reinterpret_cast<double*>(&m_mousePos.x),
			reinterpret_cast<double*>(&m_mousePos.y)
		);
		glfwSwapBuffers(m_handle);
		glfwPollEvents();
	}
	inline void   close() 
	{ 
		glfwSetWindowShouldClose(m_handle, true); 
		glfwTerminate();  
		exit(0);
	}
	inline void   clearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void clearConsole() 
	{
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { 0, 0 };

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;

		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)' ',
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) return;
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}
};