#pragma once

#include <vector>

#include "Onyx/Core/Base.h"
#include "Onyx/Core/Window.h"

#include "Onyx/Scene/Camera.h"
#include "Onyx/Scene/GameObject.h"

#include "Platform/Vulkan/Pipeline/Pipeline.h"
#include "Platform/Vulkan/Device.h"
#include "Platform/Vulkan/FrameInfo.h"

namespace Onyx
{

	class PointLightRenderSystem
	{
	public:
		PointLightRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~PointLightRenderSystem();

		PointLightRenderSystem(const PointLightRenderSystem&) = delete;
		PointLightRenderSystem& operator=(const PointLightRenderSystem&) = delete;

		void Update(FrameInfo& info, GlobalUbo& ubo);
		void Render(FrameInfo& info);

	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);

	private:
		Scope<Pipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}