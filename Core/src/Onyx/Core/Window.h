#pragma once

#include <sstream>

#include "Onyx/Core/Base.h"
#include "Onyx/Events/Event.h"

#include <vulkan/vulkan.h>

namespace Onyx
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Onyx Engine", uint32_t width = 1280, uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		virtual ~Window() = default;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual VkExtent2D GetExtent() { return { GetWidth(), GetHeight() }; }

		virtual void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());

		bool ShouldRebuildSwapChain() { return m_RebuildSwapChain; }
		void SetRebuildSwapChain(bool rebuild) { m_RebuildSwapChain = rebuild; }

		virtual bool IsMinimized() = 0;

	protected:
		bool m_RebuildSwapChain;
		bool m_Minimized;
	};
}