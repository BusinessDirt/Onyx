#include "stdafx.h"
#include "SimpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>

#include "Onyx/Core/Application.h"

#include "Onyx/Input/Input.h"
#include "Onyx/Util/PlatformUtil.h"
#include "Onyx/Core/Timestep.h"

namespace Onyx
{
	struct SimplePushConstantData
	{
		glm::mat4 ModelMatrix{ 1.0f };
		glm::mat4 NormalMatrix{ 1.0f };
	};

	SimpleRenderSystem::SimpleRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		CreatePipelineLayout(globalSetLayout);
		CreatePipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(Application::Get().GetDevice().GetHandle(), m_PipelineLayout, nullptr);
		m_Pipeline = nullptr;
	}

	void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);


		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		ONYX_VK_ASSERT(vkCreatePipelineLayout(Application::Get().GetDevice().GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout), "Failed to create pipeline layout");
	}

	void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		ONYX_CORE_ASSERT(m_PipelineLayout, "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.RenderPass = renderPass;
		pipelineConfig.PipelineLayout = m_PipelineLayout;

		m_Pipeline = CreateScope<Pipeline>("shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void SimpleRenderSystem::Render(FrameInfo& info)
	{
		m_Pipeline->Bind(info.CommandBuffer);

		vkCmdBindDescriptorSets(info.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1,
			&info.GlobalDescriptorSet, 0, nullptr);

		for (auto& [id, obj] : info.GameObjects)
		{
			if (!obj->Model) continue;

			SimplePushConstantData push{};
			push.ModelMatrix = obj->Transform.GetTransform();
			push.NormalMatrix = obj->Transform.GetNormalMatrix();

			vkCmdPushConstants(info.CommandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
				sizeof(SimplePushConstantData), &push);
			obj->Model->Bind(info.CommandBuffer);
			obj->Model->Draw(info.CommandBuffer);
		}
	}
}


