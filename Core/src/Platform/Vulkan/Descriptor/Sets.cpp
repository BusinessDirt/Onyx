#include "stdafx.h"
#include "Sets.h"

#include "Platform/Vulkan/Descriptor/Pool.h"
#include "Platform/Vulkan/Descriptor/Layout.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	DescriptorSets::DescriptorSets(const DescriptorPool& pool, const DescriptorSetLayout& layout, std::map<uint32_t, VkDescriptorType> bindingTypes, size_t size)
		: m_DescriptorPool(pool), m_BindingTypes(std::move(bindingTypes))
	{
		std::vector<VkDescriptorSetLayout> layouts(size, layout.GetHandle());

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool.GetHandle();
		allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(size);

		ONYX_VK_ASSERT(vkAllocateDescriptorSets(Application::Get().GetDevice().GetHandle(), &allocInfo, m_DescriptorSets.data()),
			"Failed to allocate descriptor sets");
	}

	DescriptorSets::~DescriptorSets() {}

	VkWriteDescriptorSet DescriptorSets::Bind(uint32_t index, uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, uint32_t count) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[index];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = GetBindingType(binding);
		descriptorWrite.descriptorCount = count;
		descriptorWrite.pBufferInfo = &bufferInfo;

		return descriptorWrite;
	}

	VkWriteDescriptorSet DescriptorSets::Bind(uint32_t index, uint32_t binding, const VkDescriptorImageInfo& imageInfo, uint32_t count) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[index];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = GetBindingType(binding);
		descriptorWrite.descriptorCount = count;
		descriptorWrite.pImageInfo = &imageInfo;

		return descriptorWrite;
	}

	void DescriptorSets::Update(uint32_t index, const std::vector<VkWriteDescriptorSet>& writes)
	{
		vkUpdateDescriptorSets(Application::Get().GetDevice().GetHandle(), static_cast<uint32_t>(writes.size()),
			writes.data(), 0, nullptr);
	}

	VkDescriptorType DescriptorSets::GetBindingType(uint32_t binding) const
	{
		const auto it = m_BindingTypes.find(binding);
		if (it == m_BindingTypes.end())
		{
			ONYX_CORE_ERROR("Binding not found");
		}

		return it->second;
	}
}
