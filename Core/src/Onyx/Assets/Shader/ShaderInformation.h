#pragma once

#include "Onyx/Core/Base.h"

#include <cstdint>
#include <vector>

namespace Onyx
{
	struct UniformBufferInformation
	{
		uint32_t Size = 0;
		uint32_t Offset = 0;
		uint32_t Binding = 0;
	};

	struct PushConstantInformation
	{
		uint32_t Size = 0;
		uint32_t Offset = 0;
	};

	struct ShaderInformation
	{
		PushConstantInformation PushConstantInformation{}; // only one push constant per shader
		std::vector<UniformBufferInformation> UniformBuffers; // multiple uniform buffers per shader
		std::vector<VkVertexInputBindingDescription> Bindings;
		std::vector<VkVertexInputAttributeDescription> Attributes;
	};
}