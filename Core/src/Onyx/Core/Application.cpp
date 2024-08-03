#include "stdafx.h"
#include "Application.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>

#include "Onyx/Input/Input.h"
#include "Onyx/Util/PlatformUtil.h"
#include "Onyx/Core/Timestep.h"

namespace Onyx
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
	{
		ONYX_CORE_ASSERT(!s_Instance, "Application already exisits!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(ONYX_BIND_EVENT_FN(Application::OnEvent));

		m_Instance = CreateScope<Instance>();
#ifdef ONYX_DEBUG
		m_DebugMessenger = CreateScope<DebugMessenger>();
#endif
		m_Surface = CreateScope<Surface>();
		m_PhysicalDevice = CreateScope<PhysicalDevice>();
		m_Device = CreateScope<Device>();
		m_CommandPool = CreateScope<CommandPool>();
		m_Renderer = CreateScope<Renderer>();
	}

	Application::~Application()
	{
		for (Layer* layer : m_Layerstack) layer->OnDetach();

		m_Renderer = nullptr;
		m_CommandPool = nullptr;
		m_Device = nullptr;
#ifdef ONYX_DEBUG
		m_DebugMessenger = nullptr;
#endif
		m_Surface = nullptr;
		m_Instance = nullptr;
		m_Window = nullptr;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(ONYX_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(ONYX_BIND_EVENT_FN(Application::OnWindowResize));

		// Handle other events
		for (std::vector<Layer*>::reverse_iterator it = m_Layerstack.rbegin(); it != m_Layerstack.rend(); ++it)
		{
			if (e.handled) break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
			m_Timestep.OnUpdate();

			if (!m_Window->IsMinimized())
			{
				for (Layer* layer : m_Layerstack)
					layer->OnUpdate((float)m_Timestep);
			}
		}

		vkDeviceWaitIdle(m_Device->GetHandle());
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (m_Window->IsMinimized()) return false;

		m_Window->SetRebuildSwapChain(true);

		return false;
	}
}


