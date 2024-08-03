#pragma once

#include <vector>

#include "Onyx/Core/Base.h"

#include "Platform/Vulkan/FrameInfo.h"

#include "Platform/Vulkan/Pipeline/Pipeline.h"
#include "Platform/Vulkan/Pipeline/PipelineLayout.h"
#include "Platform/Vulkan/Descriptor/Layout.h"

namespace Onyx
{
	class RenderSystem
	{
	public:
		RenderSystem() = default;
		~RenderSystem()
		{
			m_PipelineLayout = nullptr;
			m_Pipeline = nullptr;
		}

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		virtual void Update(FrameInfo& info, GlobalUbo& ubo) = 0;
		virtual void Render(FrameInfo& info) = 0;

	protected:
		Scope<Pipeline> m_Pipeline;
		Scope<PipelineLayout> m_PipelineLayout;
	};
}