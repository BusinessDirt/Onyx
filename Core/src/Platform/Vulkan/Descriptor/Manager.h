#pragma once

#include "Onyx/Core/Base.h"
#include "Platform/Vulkan/Descriptor/Binding.h"

namespace Onyx
{

	class DescriptorPool;
	class DescriptorSetLayout;
	class DescriptorSets;

	class DescriptorSetManager
	{
	public:
		ONYX_NON_COPIABLE(DescriptorSetManager);

		explicit DescriptorSetManager(const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
		~DescriptorSetManager();

		const DescriptorSetLayout& GetLayout() const { return *m_DescriptorSetLayout; }
		DescriptorSets& GetSets() const { return *m_DescriptorSets; }

	private:
		Scope<DescriptorPool> m_DescriptorPool;
		Scope<DescriptorSetLayout> m_DescriptorSetLayout;
		Scope<DescriptorSets> m_DescriptorSets;
	};
}