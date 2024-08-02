#include "stdafx.h"
#include "DescriptorWriter.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	DescriptorWriter::DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool)
		: m_SetLayout(setLayout), m_Pool(pool)
	{

	}

	DescriptorWriter& DescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
	{
		ONYX_CORE_ASSERT(m_SetLayout.m_Bindings.count(binding) == 1, "Layout does not contain specified binding");

		VkDescriptorSetLayoutBinding& bindingDescription = m_SetLayout.m_Bindings[binding];

		ONYX_CORE_ASSERT(bindingDescription.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		m_Writes.push_back(write);
		return *this;
	}

	DescriptorWriter& DescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
	{
		ONYX_CORE_ASSERT(m_SetLayout.m_Bindings.count(binding) == 1, "Layout does not contain specified binding");

		VkDescriptorSetLayoutBinding& bindingDescription = m_SetLayout.m_Bindings[binding];

		ONYX_CORE_ASSERT(bindingDescription.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		m_Writes.push_back(write);
		return *this;
	}

	bool DescriptorWriter::Build(VkDescriptorSet& set)
	{
		bool success = m_Pool.AllocateDescriptor(m_SetLayout.GetDescriptorSetLayout(), set);
		if (!success) return false;
		Overwrite(set);
		return true;
	}

	void DescriptorWriter::Overwrite(VkDescriptorSet& set)
	{
		for (VkWriteDescriptorSet& write : m_Writes) 
		{
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(Application::Get().GetDevice().GetHandle(), m_Writes.size(), m_Writes.data(), 0, nullptr);
	}
}
