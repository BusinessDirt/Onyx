#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class Instance
	{
	public:
		ONYX_NON_COPIABLE(Instance);

		Instance();
		~Instance();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void HasRequiredExtensions();

		const std::vector<const char*>& GetValidationLayers() const { return m_ValidationLayers; }
		const std::vector<const char*>& GetDeviceExtensions() const { return m_DeviceExtensions; }

	private:
		VULKAN_HANDLE(VkInstance, m_Instance);

		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};
}