#pragma once

#include <vector>

#include "Onyx.h"

namespace Onyx
{
	class SimpleRenderSystem : public RenderSystem
	{
	public:
		SimpleRenderSystem(const DescriptorSetLayout& globalSetLayout);

		virtual void Update(FrameInfo& info, GlobalUbo& ubo) override;
		virtual void Render(FrameInfo& info) override;
	};
}