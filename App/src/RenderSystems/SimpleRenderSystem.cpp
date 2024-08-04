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

	SimpleRenderSystem::SimpleRenderSystem(const DescriptorSetLayout& globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		m_PipelineLayout = CreateScope<PipelineLayout>(globalSetLayout, &pushConstantRange);
		
		ONYX_CORE_ASSERT(m_PipelineLayout, "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.RenderPass = Application::Get().GetRenderer().GetSwapChainRenderPass();
		pipelineConfig.PipelineLayout = m_PipelineLayout->GetHandle();

		m_Pipeline = CreateScope<Pipeline>("simple_shader.vert", "simple_shader.frag", pipelineConfig);
	}

	void SimpleRenderSystem::Update(FrameInfo& info, GlobalUbo& ubo)
	{
	}

	void SimpleRenderSystem::Render(FrameInfo& info)
	{
		m_Pipeline->Bind(info.CommandBuffer);

		vkCmdBindDescriptorSets(info.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout->GetHandle(), 0, 1,
			&info.GlobalDescriptorSet, 0, nullptr);

		for (auto& [id, obj] : info.GameObjects)
		{
			if (!obj->Model) continue;

			SimplePushConstantData push{};
			push.ModelMatrix = obj->Transform.GetTransform();
			push.NormalMatrix = obj->Transform.GetNormalMatrix();

			vkCmdPushConstants(info.CommandBuffer, m_PipelineLayout->GetHandle(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
				sizeof(SimplePushConstantData), &push);
			obj->Model->Bind(info.CommandBuffer);
			obj->Model->Draw(info.CommandBuffer);
		}
	}
}


