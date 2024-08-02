#include "stdafx.h"
#include "DescriptorSetLayout.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::AddBinding(
		uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
	{
		ONYX_CORE_ASSERT(m_Bindings.count(binding) == 0, "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stageFlags;
		m_Bindings[binding] = layoutBinding;
		return *this;
	}

	Scope<DescriptorSetLayout> DescriptorSetLayout::Builder::Build() const
	{
		return CreateScope<DescriptorSetLayout>(m_Device, m_Bindings);
	}

	DescriptorSetLayout::DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
		: m_Device(device), m_Bindings(bindings)
	{
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto kv : bindings) setLayoutBindings.push_back(kv.second);

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

		ONYX_VK_ASSERT(vkCreateDescriptorSetLayout(device.GetHandle(), &descriptorSetLayoutInfo, nullptr,&m_DescriptorSetLayout), 
			"Failed to create descriptor set layout!");
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(Application::Get().GetDevice().GetHandle(), m_DescriptorSetLayout, nullptr);
	}
}


