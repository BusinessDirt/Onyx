#pragma once

#include "Onyx/Core/Base.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_cfg.hpp>

#include <filesystem>

namespace Onyx
{

	class ShaderCode
	{
	public:
		ShaderCode(const std::filesystem::path& path, const std::filesystem::path& cachePath, bool compile);
		~ShaderCode();

		VkShaderModuleCreateInfo CreateInfo() const;

	private:
		void Compile(const std::filesystem::path& path, const std::filesystem::path& cachePath);

		void Reflect(const std::filesystem::path& path);
		void ReflectResource(const spirv_cross::Compiler& compiler, const spirv_cross::Resource& resource);
		void ReflectMember(const spirv_cross::Compiler& compiler, const spirv_cross::Resource& resource,
			const spirv_cross::SPIRType& bufferType, int index);

		std::string TypeToString(spirv_cross::SPIRType type);

	private:
		std::vector<uint32_t> m_Data;
	};
}