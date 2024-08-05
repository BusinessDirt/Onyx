#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class ShaderCode;

	class ShaderModule
	{
	public:
		ONYX_NON_COPIABLE(ShaderModule);

		ShaderModule(const ShaderCode& code);
		~ShaderModule();

	private:
		VULKAN_HANDLE(VkShaderModule, m_ShaderModule);
	};
}