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

	PointLightRenderSystem::PointLightRenderSystem(const DescriptorSetLayout& globalSetLayout)
	{
		PipelineConfigInfo configInfo{};
		Pipeline::DefaultPipelineConfigInfo(configInfo);
		configInfo.AttributeDescriptions.clear();
		configInfo.BindingDescriptions.clear();

		ONYX_INFO("Point Light Push Constant Data Size: {0}", sizeof(PointLightPushConstant));
		m_Pipeline = CreateScope<Pipeline>("point_light.vert", "point_light.frag", configInfo, globalSetLayout);
	}

	PointLightRenderSystem::~PointLightRenderSystem()
	{
		m_Pipeline = nullptr;
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
		m_Pipeline->Bind(info);

		for (auto& [id, obj] : info.GameObjects)
		{
			if (!obj->PointLight) continue;

			PointLightPushConstant push{};
			push.Position = glm::vec4(obj->Transform.Translation, 1.0f);
			push.Color = glm::vec4(obj->Color, obj->PointLight->LightIntensity);
			push.Radius = obj->Transform.Scale.x;

			m_Pipeline->PushConstants(info, &push);

			vkCmdDraw(info.CommandBuffer, 6, 1, 0, 0);
		}
	}
}