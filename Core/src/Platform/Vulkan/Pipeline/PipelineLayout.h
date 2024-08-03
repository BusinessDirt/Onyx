#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class DescriptorSetLayout;

	class PipelineLayout
	{
	public:
		ONYX_NON_COPIABLE(PipelineLayout);

		PipelineLayout(const DescriptorSetLayout& descriptorSetLayout, VkPushConstantRange* range);
		~PipelineLayout();

	private:
		VULKAN_HANDLE(VkPipelineLayout, m_PipelineLayout);
	};
}