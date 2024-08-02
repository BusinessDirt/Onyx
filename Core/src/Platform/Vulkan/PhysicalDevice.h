#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{

	struct SwapChainSupportDetails;
	struct QueueFamilyIndices;

	class PhysicalDevice
	{
	public:
		ONYX_NON_COPIABLE(PhysicalDevice);

		PhysicalDevice();
		~PhysicalDevice();

		const VkPhysicalDeviceProperties& GetProperties() const { return m_Properties; }

		SwapChainSupportDetails GetSwapChainSupport() const;
		QueueFamilyIndices FindPhysicalQueueFamilies() const;

	private:
		bool IsDeviceSuitable(VkPhysicalDevice device);

	private:
		VULKAN_HANDLE(VkPhysicalDevice, m_PhysicalDevice);
		VkPhysicalDeviceProperties m_Properties;
	};
}