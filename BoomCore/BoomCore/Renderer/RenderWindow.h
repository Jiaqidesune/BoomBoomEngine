#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera/Camera.h"

namespace Kawaii
{
	class RenderWindow
	{
	private:
		bool m_debugMode;
		int m_width, m_height;
		GLFWwindow* m_windowHandler;
		Camera* m_camera;

	public:

		// key, deltatime.
		static glm::vec2 m_cursorPos;
		static glm::vec2 m_deltaCurPos;
		static bool m_keysPressed[1024];
		static float m_deltaTime, m_lastFrame;
		static bool m_buttonPressed[GLFW_MOUSE_BUTTON_LAST];

		RenderWindow() = default;
		~RenderWindow() = default;

		bool initialize(std::string title, int width, int height, bool debugEnable = false);
		bool run();
		bool shutdown();
		void beginFrame();
		void endFrame();

		int getWindowWidth()const { return m_width; }
		int getWindowHeight()const { return m_height; }

	private:
		void initializeDebugContex();
		static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar* message, const void* userParam);

		static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void processInput();
	};


}