#pragma once

#include "Onyx/Core/Base.h"
#include "Platform/Vulkan/Descriptor/Binding.h"

namespace Onyx
{
	class DescriptorSetLayout
	{
	public:
		
		ONYX_NON_COPIABLE(DescriptorSetLayout);

		DescriptorSetLayout(const std::vector<DescriptorBinding>& descriptorBindings);
		~DescriptorSetLayout();

	private:
		VULKAN_HANDLE(VkDescriptorSetLayout, m_Layout);
	};
}