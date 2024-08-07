#include "stdafx.h"
#include "GraphicsPipeline.h"

#include "Onyx/Scene/Model.h"

#include "Onyx/Core/Application.h"

#include "Platform/Vulkan/ShaderModule.h"

#include <fstream>

namespace Onyx
{
	GraphicsPipeline::GraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo& configInfo, const DescriptorSetLayout& descriptorSetLayout)
	{
		const ShaderCode& vertexCode = Application::Get().GetAssetManager().GetShader(vertFilepath);
		const ShaderCode& fragmentCode = Application::Get().GetAssetManager().GetShader(fragFilepath);

		// Pipeline Layout
		{
			// invalid if both push constant sizes are greater than 0 and not equal
			uint32_t vertexPushConstantSize = vertexCode.GetInformation().PushConstantInformation.Size;
			uint32_t fragmentPushConstantSize = fragmentCode.GetInformation().PushConstantInformation.Size;
			ONYX_VK_ASSERT(vertexPushConstantSize > 0 && fragmentPushConstantSize > 0 && vertexPushConstantSize != fragmentPushConstantSize,
				"Invalid push constant definition in shader");

			m_PushConstantInfo.Size = vertexPushConstantSize;
			m_PushConstantInfo.Offset = vertexCode.GetInformation().PushConstantInformation.Offset;
			// push constant can be available in one shader or both
			m_PushConstantInfo.Flags = (vertexPushConstantSize > 0 ? VK_SHADER_STAGE_VERTEX_BIT : 0) |
				(fragmentPushConstantSize > 0 ? VK_SHADER_STAGE_FRAGMENT_BIT : 0);

			VkPushConstantRange pushConstantRange{};
			pushConstantRange.stageFlags = m_PushConstantInfo.Flags;
			pushConstantRange.offset = m_PushConstantInfo.Offset;
			pushConstantRange.size = m_PushConstantInfo.Size;

			m_PipelineLayout = CreateScope<PipelineLayout>(descriptorSetLayout, &pushConstantRange);
		}

		// Pipeline
		{
			// retrieve binding and attribute descriptions from vertex shader code
			const std::vector<VkVertexInputBindingDescription>& bindingDescriptions = vertexCode.GetInformation().Bindings;
			const std::vector<VkVertexInputAttributeDescription>& attributeDescriptions = vertexCode.GetInformation().Attributes;

			ShaderModule vertexShader(vertexCode);
			ShaderModule fragmentShader(fragmentCode);

			VkPipelineShaderStageCreateInfo shaderStages[2];
			shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
			shaderStages[0].module = vertexShader.GetHandle();
			shaderStages[0].pName = "main";
			shaderStages[0].flags = 0;
			shaderStages[0].pNext = nullptr;
			shaderStages[0].pSpecializationInfo = nullptr;
			shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			shaderStages[1].module = fragmentShader.GetHandle();
			shaderStages[1].pName = "main";
			shaderStages[1].flags = 0;
			shaderStages[1].pNext = nullptr;
			shaderStages[1].pSpecializationInfo = nullptr;

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
			vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &configInfo.InputAssemblyInfo;
			pipelineInfo.pViewportState = &configInfo.ViewportInfo;
			pipelineInfo.pRasterizationState = &configInfo.RasterizationInfo;
			pipelineInfo.pMultisampleState = &configInfo.MultisampleInfo;
			pipelineInfo.pColorBlendState = &configInfo.ColorBlendInfo;
			pipelineInfo.pDepthStencilState = &configInfo.DepthStencilInfo;
			pipelineInfo.pDynamicState = &configInfo.DynamicStateInfo;

			pipelineInfo.layout = m_PipelineLayout->GetHandle();
			pipelineInfo.renderPass = Application::Get().GetRenderer().GetSwapChainRenderPass();
			pipelineInfo.subpass = configInfo.Subpass;

			pipelineInfo.basePipelineIndex = -1;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			ONYX_VK_ASSERT(vkCreateGraphicsPipelines(Application::Get().GetDevice().GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline),
				"Failed to create graphics pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		m_PipelineLayout = nullptr;
		vkDestroyPipeline(Application::Get().GetDevice().GetHandle(), m_GraphicsPipeline, nullptr);
	}

	void GraphicsPipeline::Bind(const FrameInfo& info)
	{
		vkCmdBindPipeline(info.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
		vkCmdBindDescriptorSets(info.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout->GetHandle(), 0, 1,
			&info.GlobalDescriptorSet, 0, nullptr);
	}

	void GraphicsPipeline::PushConstants(const FrameInfo& info, const void* pValues)
	{
		vkCmdPushConstants(info.CommandBuffer, m_PipelineLayout->GetHandle(), m_PushConstantInfo.Flags, m_PushConstantInfo.Offset, m_PushConstantInfo.Size, pValues);
	}

	void GraphicsPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
	{
		configInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.ViewportInfo.viewportCount = 1;
		configInfo.ViewportInfo.pViewports = nullptr;
		configInfo.ViewportInfo.scissorCount = 1;
		configInfo.ViewportInfo.pScissors = nullptr;

		configInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.RasterizationInfo.lineWidth = 1.0f;
		configInfo.RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		configInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.MultisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.MultisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		configInfo.ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo.ColorBlendInfo.attachmentCount = 1;
		configInfo.ColorBlendInfo.pAttachments = &configInfo.ColorBlendAttachment;
		configInfo.ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo.ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo.ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo.ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		configInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.front = {};  // Optional
		configInfo.DepthStencilInfo.back = {};   // Optional

		configInfo.DynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo.DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.DynamicStateInfo.pDynamicStates = configInfo.DynamicStateEnables.data();
		configInfo.DynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.DynamicStateEnables.size());
		configInfo.DynamicStateInfo.flags = 0;
	}

	std::vector<char> GraphicsPipeline::ReadFile(const std::string& filepath)
	{
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };
		ONYX_CORE_ASSERT(file.is_open(), "Could not open file " + filepath);

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}
}
