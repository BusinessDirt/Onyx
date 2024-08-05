#pragma once

#include "Onyx/Core/Base.h"

#include "Platform/Vulkan/Device.h"
#include "Platform/Vulkan/Pipeline/PipelineLayout.h"
#include "Platform/Vulkan/FrameInfo.h"

#include "Onyx/Assets/Shader/ShaderCode.h"

#include <string>
#include <vector>

namespace Onyx
{

	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		ONYX_NON_COPIABLE(PipelineConfigInfo);

		VkPipelineViewportStateCreateInfo ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;

		std::vector<VkDynamicState> DynamicStateEnables;
		VkPipelineDynamicStateCreateInfo DynamicStateInfo;

		uint32_t Subpass = 0;
	};

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo& configInfo, const DescriptorSetLayout& descriptorSetLayout);
		~GraphicsPipeline();

		GraphicsPipeline(const GraphicsPipeline&) = delete;
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

		void Bind(const FrameInfo& info);
		void PushConstants(const FrameInfo& info, const void* pValues);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> ReadFile(const std::string& filepath);

		void CreateShaderModule(const ShaderCode& code, VkShaderModule* shaderModule);

	private:
		struct PushConstantInfo
		{
			uint32_t Size;
			uint32_t Offset;
			VkShaderStageFlags Flags;
		};

	private:
		Scope<PipelineLayout> m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;

		PushConstantInfo m_PushConstantInfo{};
	};
}