#pragma once

#include "Onyx/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Onyx
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		bool IsMinimized() override { return m_Data.Minimized; }

		virtual void* GetNativeWindow() const { return m_Window; }

		virtual void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) override;

	private:
		GLFWwindow* m_Window;

		struct WindowData {
			std::string Title;
			unsigned int Width = 0, Height = 0;
			bool VSync = true;
			bool Minimized = false;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}