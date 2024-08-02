#include "stdafx.h"
#include "Manager.h"

#include "Platform/Vulkan/Descriptor/Pool.h"
#include "Platform/Vulkan/Descriptor/Layout.h"
#include "Platform/Vulkan/Descriptor/Sets.h"

namespace Onyx
{
	DescriptorSetManager::DescriptorSetManager(const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets)
	{
		std::map<uint32_t, VkDescriptorType> bindingTypes;

		for (const DescriptorBinding& binding : descriptorBindings)
		{
			if (!bindingTypes.insert(std::make_pair(binding.Binding, binding.Type)).second)
			{
				ONYX_CORE_ERROR("Binding collision");
			}
		}

		m_DescriptorPool.reset(new DescriptorPool(descriptorBindings, maxSets));
		m_DescriptorSetLayout.reset(new DescriptorSetLayout(descriptorBindings));
		m_DescriptorSets.reset(new DescriptorSets(*m_DescriptorPool, *m_DescriptorSetLayout, bindingTypes, maxSets));
	}

	DescriptorSetManager::~DescriptorSetManager()
	{
		m_DescriptorSets = nullptr;
		m_DescriptorSetLayout = nullptr;
		m_DescriptorPool = nullptr;
	}
}
