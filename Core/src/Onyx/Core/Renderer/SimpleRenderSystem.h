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

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void Render(FrameInfo& info);

	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);

	private:
		Scope<Pipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}