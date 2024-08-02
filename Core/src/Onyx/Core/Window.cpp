#include "stdafx.h"
#include "Window.h"

#ifdef ONYX_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Onyx
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
		
#ifdef ONYX_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		ONYX_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}
