#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class Instance;

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    struct QueueFamilyIndices
    {
        uint32_t GraphicsFamily;
        uint32_t PresentFamily;
        bool GraphicsFamilyHasValue = false;
        bool PresentFamilyHasValue = false;
        bool IsComplete()
        {
            return GraphicsFamilyHasValue && PresentFamilyHasValue;
        }
    };

	class Surface
	{
	public:
		ONYX_NON_COPIABLE(Surface);

		Surface();
		~Surface();

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;

	private:
		VULKAN_HANDLE(VkSurfaceKHR, m_Surface);
	};
}