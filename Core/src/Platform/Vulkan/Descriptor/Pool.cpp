#include "stdafx.h"
#include "Pool.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{

	DescriptorPool::DescriptorPool(const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets)
	{
		std::vector<VkDescriptorPoolSize> poolSizes;

		for (const DescriptorBinding& binding : descriptorBindings)
			poolSizes.push_back(VkDescriptorPoolSize{ binding.Type, static_cast<uint32_t>(binding.DescriptorCount * maxSets) });

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(maxSets);

		ONYX_VK_ASSERT(vkCreateDescriptorPool(Application::Get().GetDevice().GetHandle(), &poolInfo, nullptr, &m_DescriptorPool), 
			"Failed to create descriptor pool");
	}

	DescriptorPool::~DescriptorPool()
	{
		if (m_DescriptorPool != nullptr)
		{
			vkDestroyDescriptorPool(Application::Get().GetDevice().GetHandle(), m_DescriptorPool, nullptr);
			m_DescriptorPool = nullptr;
		}
	}
}

