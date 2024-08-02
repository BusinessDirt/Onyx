#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{

	class DescriptorPool;
	class DescriptorSetLayout;

	class DescriptorSets
	{
	public:
		ONYX_NON_COPIABLE(DescriptorSets);

		DescriptorSets(const DescriptorPool& pool, const DescriptorSetLayout& layout, std::map<uint32_t, VkDescriptorType> bindingTypes, size_t size);
		~DescriptorSets();

		VkDescriptorSet GetHandle(uint32_t index) const { return m_DescriptorSets[index]; }

		VkWriteDescriptorSet Bind(uint32_t index, uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, uint32_t count = 1) const;
		VkWriteDescriptorSet Bind(uint32_t index, uint32_t binding, const VkDescriptorImageInfo& imageInfo, uint32_t count = 1) const;

		void Update(uint32_t index, const std::vector<VkWriteDescriptorSet>& writes);

	private:
		VkDescriptorType GetBindingType(uint32_t binding) const;

	private:
		const DescriptorPool& m_DescriptorPool;
		const std::map<uint32_t, VkDescriptorType> m_BindingTypes;

		std::vector<VkDescriptorSet> m_DescriptorSets;
	};
}