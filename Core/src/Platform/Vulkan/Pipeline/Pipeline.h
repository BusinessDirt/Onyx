#pragma once

#include "Onyx/Core/Base.h"

#include "Platform/Vulkan/Device.h"

#include <string>
#include <vector>

namespace Onyx
{

	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		ONYX_NON_COPIABLE(PipelineConfigInfo);

		std::vector<VkVertexInputBindingDescription> BindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> AttributeDescriptions{};
		VkPipelineViewportStateCreateInfo ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;

		std::vector<VkDynamicState> DynamicStateEnables;
		VkPipelineDynamicStateCreateInfo DynamicStateInfo;

		VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
		VkRenderPass RenderPass = VK_NULL_HANDLE;
		uint32_t Subpass = 0;
	};

	class Pipeline
	{
	public:
		Pipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> ReadFile(const std::string& filepath);
		void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	private:
		VkPipeline m_GraphicsPipeline;
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;
	};
}