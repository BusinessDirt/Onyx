#include "stdafx.h"
#include "WindowsWindow.h"

#include "Onyx/Core/Base.h"
#include "Onyx/Input/Input.h"

#include "Onyx/Events/ApplicationEvent.h"
#include "Onyx/Events/MouseEvent.h"
#include "Onyx/Events/KeyEvent.h"

namespace Onyx
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ONYX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ONYX_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			ONYX_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			ONYX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			// Create the window
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);

		// CALLBACKS
		{
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					data.Width = width;
					data.Height = height;
					
					if (width == 0 || height == 0) data.Minimized = true;

					WindowResizeEvent event(width, height);
					data.EventCallback(event);
				});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					WindowCloseEvent event;
					data.EventCallback(event);
				});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
						case GLFW_PRESS:
						{
							KeyPressedEvent event(key);
							data.EventCallback(event);
							break;
						}
						case GLFW_RELEASE:
						{
							KeyReleasedEvent event(key);
							data.EventCallback(event);
							break;
						}
						case GLFW_REPEAT:
						{
							KeyPressedEvent event(key, true);
							data.EventCallback(event);
							break;
						}
					}
				});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) 
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					KeyTypedEvent event(keycode);
					data.EventCallback(event);
				});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
						case GLFW_PRESS:
						{
							MouseButtonPressedEvent event(button);
							data.EventCallback(event);
							break;
						}
						case GLFW_RELEASE:
						{
							MouseButtonReleasedEvent event(button);
							data.EventCallback(event);
							break;
						}
					}
				});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) 
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseScrolledEvent event((float)xOffset, (float)yOffset);
					data.EventCallback(event);
				});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) 
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseMovedEvent event((float)xPos, (float)yPos);
					data.EventCallback(event);
				});
		}
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			ONYX_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		ONYX_VK_ASSERT(glfwCreateWindowSurface(instance, m_Window, nullptr, surface), "Failed to create window surface");
	}
}
