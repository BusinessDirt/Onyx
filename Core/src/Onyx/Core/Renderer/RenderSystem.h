#pragma once

#include <vector>

#include "Onyx/Core/Base.h"

#include "Platform/Vulkan/FrameInfo.h"

#include "Platform/Vulkan/Pipeline/GraphicsPipeline.h"
#include "Platform/Vulkan/Descriptor/Layout.h"

namespace Onyx
{
	class RenderSystem
	{
	public:
		RenderSystem() = default;
		virtual ~RenderSystem() = default;

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		virtual void Update(FrameInfo& info, GlobalUbo& ubo) = 0;
		virtual void Render(FrameInfo& info) = 0;

	protected:
		Scope<GraphicsPipeline> m_Pipeline;
	};
}