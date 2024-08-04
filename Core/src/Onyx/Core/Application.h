#pragma once

#include <vector>

#include "Onyx/Core/Base.h"
#include "Onyx/Core/Timestep.h"
#include "Onyx/Core/Window.h"
#include "Onyx/Core/Renderer/Renderer.h"
#include "Onyx/Core/Layers/LayerStack.h"

#include "Onyx/Events/Event.h"
#include "Onyx/Events/ApplicationEvent.h"
#include "Onyx/Events/KeyEvent.h"
#include "Onyx/Events/MouseEvent.h"

#include "Onyx/Scene/Camera.h"
#include "Onyx/Scene/GameObject.h"

#include "Onyx/Assets/AssetManager.h"

#include "Platform/Vulkan/Instance.h"
#include "Platform/Vulkan/DebugMessenger.h"
#include "Platform/Vulkan/Surface.h"
#include "Platform/Vulkan/PhysicalDevice.h"
#include "Platform/Vulkan/Device.h"
#include "Platform/Vulkan/CommandPool.h"

int Entrypoint(int argc, char** argv);

namespace Onyx
{

	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const
		{
			ONYX_CORE_ASSERT(index < count);
			return args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "Onyx App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void OnEvent(Event& e);

		void PushLayer(Layer* layer) { m_Layerstack.PushLayer(layer); }

		void Close();

		AssetManager& GetAssetManager() { return *m_AssetManager; }
		Window& GetWindow() { return *m_Window; }
		Instance& GetInstance() { return *m_Instance; }
		DebugMessenger& GetDebugMessenger() { return *m_DebugMessenger; }
		Surface& GetSurface() { return *m_Surface; }
		PhysicalDevice& GetPhysicalDevice() { return *m_PhysicalDevice; }
		Device& GetDevice() { return *m_Device; }
		CommandPool& GetCommandPool() { return *m_CommandPool; }
		Renderer& GetRenderer() { return *m_Renderer; }

		static Application& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<AssetManager> m_AssetManager;
		Scope<Window> m_Window;
		Scope<Instance> m_Instance;
		Scope<DebugMessenger> m_DebugMessenger;
		Scope<Surface> m_Surface;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Device> m_Device;
		Scope<CommandPool> m_CommandPool;
		Scope<Renderer> m_Renderer;

		bool m_Running = true;
		Timestep m_Timestep{};

		LayerStack m_Layerstack{};

	private:
		static Application* s_Instance;
		friend int ::Entrypoint(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}