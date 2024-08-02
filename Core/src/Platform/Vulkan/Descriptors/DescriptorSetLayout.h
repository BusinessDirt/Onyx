#pragma once

#include "Onyx/Core/Base.h"
#include "Platform/Vulkan/Device.h"

namespace Onyx
{
	class DescriptorSetLayout
	{
	public:
		class Builder
		{
		public:
			Builder(Device& device) 
				: m_Device(device) 
			{}

			Builder& AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
			Scope<DescriptorSetLayout> Build() const;

		private:
			Device& m_Device;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
		};

		DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~DescriptorSetLayout();
		DescriptorSetLayout(const DescriptorSetLayout&) = delete;
		DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }

	private:
		Device& m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

		friend class DescriptorWriter;
	};
}