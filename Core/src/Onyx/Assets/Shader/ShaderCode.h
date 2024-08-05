#pragma once

#include "Onyx/Core/Base.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_cfg.hpp>

#include <filesystem>

#include "Onyx/Assets/Shader/ShaderInformation.h"

namespace Onyx
{

	class ShaderCode
	{
	public:
		ShaderCode(const std::filesystem::path& path, const std::filesystem::path& cachePath, bool compile);
		~ShaderCode();

		VkShaderModuleCreateInfo CreateInfo() const;
		const ShaderInformation& GetInformation() const { return m_ShaderInformation; }

	private:
		void Compile(const std::filesystem::path& path, const std::filesystem::path& cachePath);

		void Reflect(const std::filesystem::path& path);

	private:
		std::vector<uint32_t> m_Data;
		ShaderInformation m_ShaderInformation{};
	};
}