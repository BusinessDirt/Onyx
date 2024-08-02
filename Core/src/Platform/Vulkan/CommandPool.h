#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class PhysicalDevice;
	class Surface;

	class CommandPool
	{
	public:
		ONYX_NON_COPIABLE(CommandPool);

		CommandPool();
		~CommandPool();

	private:
		VULKAN_HANDLE(VkCommandPool, m_CommandPool);
	};
}