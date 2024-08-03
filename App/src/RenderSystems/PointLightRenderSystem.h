#pragma once

#include "Onyx.h"

namespace Onyx
{
	class PointLightRenderSystem : public RenderSystem
	{
	public:
		PointLightRenderSystem(const DescriptorSetLayout& globalSetLayout);
		
		virtual void Update(FrameInfo& info, GlobalUbo& ubo) override;
		virtual void Render(FrameInfo& info) override;
	};
}