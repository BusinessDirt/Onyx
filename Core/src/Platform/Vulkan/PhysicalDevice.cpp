#include "stdafx.h"
#include "PhysicalDevice.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	PhysicalDevice::PhysicalDevice()
	{
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(Application::Get().GetInstance().GetHandle(), &deviceCount, nullptr);
        ONYX_CORE_ASSERT(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

        ONYX_CORE_INFO("Device count: {0}", deviceCount);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(Application::Get().GetInstance().GetHandle(), &deviceCount, devices.data());

        for (const auto& device : devices)
        {
            if (IsDeviceSuitable(device))
            {
                m_PhysicalDevice = device;
                break;
            }
        }

        ONYX_CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!");

        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
        ONYX_CORE_INFO("Physical Device: {0}", m_Properties.deviceName);
	}

	PhysicalDevice::~PhysicalDevice() {}

    SwapChainSupportDetails PhysicalDevice::GetSwapChainSupport() const
    {
        return Application::Get().GetSurface().QuerySwapChainSupport(m_PhysicalDevice);
    }

    QueueFamilyIndices PhysicalDevice::FindPhysicalQueueFamilies() const
    {
        return Application::Get().GetSurface().FindQueueFamilies(m_PhysicalDevice);
    }

    bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = Application::Get().GetSurface().FindQueueFamilies(device);

		bool extensionsSupported = Application::Get().GetSurface().CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = Application::Get().GetSurface().QuerySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}
}
