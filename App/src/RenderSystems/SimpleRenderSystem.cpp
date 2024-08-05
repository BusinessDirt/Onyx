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
		PipelineConfigInfo configInfo{};
		GraphicsPipeline::DefaultPipelineConfigInfo(configInfo);

		ONYX_INFO("Simple Push Constant Data Size: {0}", sizeof(SimplePushConstantData));
		m_Pipeline = CreateScope<GraphicsPipeline>("simple_shader.vert", "simple_shader.frag", configInfo, globalSetLayout);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		m_Pipeline = nullptr;
	}

	void SimpleRenderSystem::Update(FrameInfo& info, GlobalUbo& ubo)
	{
	}

	void SimpleRenderSystem::Render(FrameInfo& info)
	{
		m_Pipeline->Bind(info);

		for (auto& [id, obj] : info.GameObjects)
		{
			if (!obj->Model) continue;

			SimplePushConstantData push{};
			push.ModelMatrix = obj->Transform.GetTransform();
			push.NormalMatrix = obj->Transform.GetNormalMatrix();

			m_Pipeline->PushConstants(info, &push);
			obj->Model->Bind(info.CommandBuffer);
			obj->Model->Draw(info.CommandBuffer);
		}
	}
}


