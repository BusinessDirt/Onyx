#pragma once

#include "Onyx/Core/Base.h"
#include "Platform/Vulkan/Descriptors/DescriptorSetLayout.h"
#include "Platform/Vulkan/Descriptors/DescriptorPool.h"

namespace Onyx
{
	class DescriptorWriter 
	{
	public:
		DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool);

		DescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		DescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool Build(VkDescriptorSet& set);
		void Overwrite(VkDescriptorSet& set);

	private:
		DescriptorSetLayout& m_SetLayout;
		DescriptorPool& m_Pool;
		std::vector<VkWriteDescriptorSet> m_Writes;
	};
}
