#include "stdafx.h"
#include "CommandPool.h"

#include "Onyx/Core/Application.h"

#include "Platform/Vulkan/Surface.h"
#include "Platform/Vulkan/PhysicalDevice.h"
#include "Platform/Vulkan/Device.h"

namespace Onyx
{
	CommandPool::CommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = Application::Get().GetPhysicalDevice().FindPhysicalQueueFamilies();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		ONYX_VK_ASSERT(vkCreateCommandPool(Application::Get().GetDevice().GetHandle(), &poolInfo, nullptr, &m_CommandPool), "Failed to create command pool");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(Application::Get().GetDevice().GetHandle(), m_CommandPool, nullptr);
	}
}
