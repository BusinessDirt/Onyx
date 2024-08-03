#include "stdafx.h"
#include "PipelineLayout.h"

#include "Onyx/Core/Application.h"

#include "Platform/Vulkan/Descriptor/Layout.h"

namespace Onyx
{
	PipelineLayout::PipelineLayout(const DescriptorSetLayout& descriptorSetLayout, VkPushConstantRange* range)
	{
		VkDescriptorSetLayout descriptorSetLayouts[] = { descriptorSetLayout.GetHandle() };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts;
		pipelineLayoutInfo.pushConstantRangeCount = range ? 1 : 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = range; // Optional

		ONYX_VK_ASSERT(vkCreatePipelineLayout(Application::Get().GetDevice().GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout),
			"Failed to create pipeline layout");
	}

	PipelineLayout::~PipelineLayout()
	{
		if (m_PipelineLayout != nullptr)
		{
			vkDestroyPipelineLayout(Application::Get().GetDevice().GetHandle(), m_PipelineLayout, nullptr);
			m_PipelineLayout = nullptr;
		}
	}
}

