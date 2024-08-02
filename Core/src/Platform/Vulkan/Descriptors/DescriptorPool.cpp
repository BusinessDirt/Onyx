#include "stdafx.h"
#include "DescriptorPool.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	DescriptorPool::Builder& DescriptorPool::Builder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count)
	{
		m_PoolSizes.push_back({ descriptorType, count });
		return *this;
	}

	DescriptorPool::Builder& DescriptorPool::Builder::SetPoolFlags(VkDescriptorPoolCreateFlags flags)
	{
		m_PoolFlags = flags;
		return *this;
	}

	DescriptorPool::Builder& DescriptorPool::Builder::SetMaxSets(uint32_t count)
	{
		m_MaxSets = count;
		return *this;
	}

	Scope<DescriptorPool> DescriptorPool::Builder::Build() const
	{
		return CreateScope<DescriptorPool>(m_MaxSets, m_PoolFlags, m_PoolSizes);
	}

	DescriptorPool::DescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		ONYX_VK_ASSERT(vkCreateDescriptorPool(Application::Get().GetDevice().GetHandle(), &descriptorPoolInfo, nullptr, &m_DescriptorPool), "failed to create descriptor pool!");
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(Application::Get().GetDevice().GetHandle(), m_DescriptorPool, nullptr);
	}

	bool DescriptorPool::AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;

		// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
		// a new pool whenever an old pool fills up. But this is beyond our current scope
		if (vkAllocateDescriptorSets(Application::Get().GetDevice().GetHandle(), &allocInfo, &descriptor) != VK_SUCCESS) return false;
		return true;
	}

	void DescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
	{
		vkFreeDescriptorSets(Application::Get().GetDevice().GetHandle(), m_DescriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
	}

	void DescriptorPool::ResetPool()
	{
		vkResetDescriptorPool(Application::Get().GetDevice().GetHandle(), m_DescriptorPool, 0);
	}
}

