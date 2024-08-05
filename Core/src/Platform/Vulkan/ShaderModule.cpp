#include "stdafx.h"
#include "ShaderModule.h"

#include "Onyx/Assets/Shader/ShaderCode.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	ShaderModule::ShaderModule(const ShaderCode& code)
	{
		VkShaderModuleCreateInfo createInfo = code.CreateInfo();

		ONYX_VK_ASSERT(vkCreateShaderModule(Application::Get().GetDevice().GetHandle(), &createInfo, nullptr, &m_ShaderModule),
			"Failed to create shader module");
	}

	ShaderModule::~ShaderModule()
	{
		vkDestroyShaderModule(Application::Get().GetDevice().GetHandle(), m_ShaderModule, nullptr);
	}
}

