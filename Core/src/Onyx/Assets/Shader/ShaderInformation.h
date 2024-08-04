#pragma once

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
		std::vector<UniformBufferInformation> UniformBuffers; // multiple uniform buffers per shader
		PushConstantInformation PushConstantInformation{}; // only one push constant per shader
	};
}