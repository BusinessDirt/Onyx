#include "stdafx.h"
#include "Layout.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	DescriptorSetLayout::DescriptorSetLayout(const std::vector<DescriptorBinding>& descriptorBindings)
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		for (const DescriptorBinding& binding : descriptorBindings)
		{
			VkDescriptorSetLayoutBinding layoutBinding{};
			layoutBinding.binding = binding.Binding;
			layoutBinding.descriptorCount = binding.DescriptorCount;
			layoutBinding.descriptorType = binding.Type;
			layoutBinding.stageFlags = binding.Stage;

			layoutBindings.push_back(layoutBinding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();

		ONYX_VK_ASSERT(vkCreateDescriptorSetLayout(Application::Get().GetDevice().GetHandle(), &layoutInfo, nullptr, &m_Layout),
			"Failed to create descriptor set layout");
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		if (m_Layout != nullptr)
		{
			vkDestroyDescriptorSetLayout(Application::Get().GetDevice().GetHandle(), m_Layout, nullptr);
			m_Layout = nullptr;
		}
	}
}


