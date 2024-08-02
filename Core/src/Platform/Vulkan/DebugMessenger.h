#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class Instance;

	class DebugMessenger
	{
	public:
		DebugMessenger();
		~DebugMessenger();

		static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	private:
		VULKAN_HANDLE(VkDebugUtilsMessengerEXT, m_DebugMessenger);
	};
}