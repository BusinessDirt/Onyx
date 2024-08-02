#include "stdafx.h"
#include "PointLightRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>

#include "Onyx/Input/Input.h"
#include "Onyx/Util/PlatformUtil.h"
#include "Onyx/Core/Timestep.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{

	struct PointLightPushConstant
	{
		glm::vec4 Position{};
		glm::vec4 Color{};
		float Radius;
	};

	PointLightRenderSystem::PointLightRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		CreatePipelineLayout(globalSetLayout);
		CreatePipeline(renderPass);
	}

	PointLightRenderSystem::~PointLightRenderSystem()
	{
		vkDestroyPipelineLayout(Application::Get().GetDevice().GetHandle(), m_PipelineLayout, nullptr);
		m_Pipeline = nullptr;
	}

	void PointLightRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PointLightPushConstant);


		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		ONYX_VK_ASSERT(vkCreatePipelineLayout(Application::Get().GetDevice().GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout), "Failed to create pipeline layout");
	}

	void PointLightRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		ONYX_CORE_ASSERT(m_PipelineLayout, "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.RenderPass = renderPass;
		pipelineConfig.PipelineLayout = m_PipelineLayout;
		pipelineConfig.AttributeDescriptions.clear();
		pipelineConfig.BindingDescriptions.clear();

		m_Pipeline = CreateScope<Pipeline>("shaders/point_light.vert.spv", "shaders/point_light.frag.spv", pipelineConfig);
	}

	void PointLightRenderSystem::Update(FrameInfo& info, GlobalUbo& ubo)
	{
		int lightIndex = 0;
		for (auto& [id, obj] : info.GameObjects)
		{
			if (!obj->PointLight) continue;
			ONYX_CORE_ASSERT(lightIndex < MAX_LIGHTS, "Point lights exceed maximum specified!");

			glm::mat4 rotateLight = glm::rotate(glm::mat4(1.0f), info.FrameTime, glm::vec3(0.0f, -1.0f, 0.0f));
			obj->Transform.Translation = glm::vec3(rotateLight * glm::vec4(obj->Transform.Translation, 1.0f));

			ubo.PointLights[lightIndex].Position = glm::vec4(obj->Transform.Translation, 1.0f);
			ubo.PointLights[lightIndex].Color = glm::vec4(obj->Color, obj->PointLight->LightIntensity);
			lightIndex++;
		}

		ubo.NumLights = lightIndex;
	}

	void PointLightRenderSystem::Render(FrameInfo& info)
	{
		m_Pipeline->Bind(info.CommandBuffer);

		vkCmdBindDescriptorSets(info.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1,
			&info.GlobalDescriptorSet, 0, nullptr);

		for (auto& [id, obj] : info.GameObjects)
		{
			if (!obj->PointLight) continue;

			PointLightPushConstant push{};
			push.Position = glm::vec4(obj->Transform.Translation, 1.0f);
			push.Color = glm::vec4(obj->Color, obj->PointLight->LightIntensity);
			push.Radius = obj->Transform.Scale.x;

			vkCmdPushConstants(info.CommandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0, sizeof(PointLightPushConstant), &push);

			vkCmdDraw(info.CommandBuffer, 6, 1, 0, 0);
		}
	}
}