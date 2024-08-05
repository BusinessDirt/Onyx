#include "stdafx.h"
#include "ShaderCode.h"

#include "Onyx/Util/Util.h"

namespace Onyx
{

	namespace Util
	{
		static std::string ReadFile(const std::filesystem::path& filepath) {
			std::string result;
			std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
			if (in) {
				in.seekg(0, std::ios::end);
				size_t size = in.tellg();
				if (size != -1) {
					result.resize(size);
					in.seekg(0, std::ios::beg);
					in.read(&result[0], size);
				}
				else ONYX_CORE_ERROR("Could not read from file '{0}'", filepath.string().c_str());

			}
			else ONYX_CORE_ERROR("Could not open file '{0}'", filepath.string().c_str());

			return result;
		}

		static shaderc_shader_kind ShaderStageToShaderC(const std::filesystem::path& path)
		{
			std::string filename = path.filename().string();
			if (Contains(filename, "vert"))
			{
				return shaderc_glsl_vertex_shader;
			}
			else if (Contains(filename, "frag"))
			{
				return shaderc_glsl_fragment_shader;
			}
		}

		static std::string ShaderStageToString(const std::filesystem::path& path)
		{
			std::string filename = path.filename().string();
			if (Contains(filename, "vert"))
			{
				return "GLSL_VERTEX_SHADER";
			}
			else if (Contains(filename, "frag"))
			{
				return "GLSL_FRAGMENT_SHADER";
			}
		}
	}

	ShaderCode::ShaderCode(const std::filesystem::path& path, const std::filesystem::path& cachePath, bool compile)
	{
		if (compile)
		{
			Compile(path, cachePath);
		}
		else
		{
			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open()) 
			{
				in.seekg(0, std::ios::end);
				std::streampos size = in.tellg();
				in.seekg(0, std::ios::beg);

				std::vector<uint32_t>& data = m_Data;
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
		}

		Reflect(path);
	}

	ShaderCode::~ShaderCode()
	{
	}

	VkShaderModuleCreateInfo ShaderCode::CreateInfo() const
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = m_Data.size() * 4;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(m_Data.data());
		return createInfo;
	}

	void ShaderCode::Compile(const std::filesystem::path& path, const std::filesystem::path& cachePath)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		shaderc_shader_kind shaderKind = Util::ShaderStageToShaderC(path);

		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

		std::string source = Util::ReadFile(path);

		auto preProcessed = compiler.PreprocessGlsl(source, shaderKind, path.filename().string().c_str(), options);
		auto compiled = compiler.CompileGlslToSpv(source, shaderKind, path.filename().string().c_str(), options);


		if (compiled.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			ONYX_CORE_ERROR(compiled.GetErrorMessage());
			ONYX_CORE_ASSERT(false);
		}

		m_Data = std::vector<uint32_t>(compiled.cbegin(), compiled.cend());

		std::ofstream out(cachePath, std::ios::out | std::ios::binary);
		if (out.is_open()) 
		{
			std::vector<uint32_t>& data = m_Data;
			out.write((char*)data.data(), data.size() * sizeof(uint32_t));
			out.flush();
			out.close();
		}
	}

	void ShaderCode::Reflect(const std::filesystem::path& path)
	{
		spirv_cross::Compiler compiler(m_Data);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		// Uniform buffers
		for (const spirv_cross::Resource& resource : resources.uniform_buffers)
		{
			const spirv_cross::SPIRType& bufferType = compiler.get_type(resource.base_type_id);

			UniformBufferInformation bufferInfo{};
			bufferInfo.Size = compiler.get_declared_struct_size(bufferType);
			bufferInfo.Binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			compiler.get_binary_offset_for_decoration(resource.id, spv::DecorationBinding, bufferInfo.Offset);
			m_ShaderInformation.UniformBuffers.push_back(bufferInfo);
		}

		// Push Constants
		for (const spirv_cross::Resource& resource : resources.push_constant_buffers)
		{
			const spirv_cross::SPIRType& bufferType = compiler.get_type(resource.base_type_id);

			m_ShaderInformation.PushConstantInformation.Size = compiler.get_declared_struct_size(bufferType);
			compiler.get_binary_offset_for_decoration(resource.id, spv::DecorationBinding, m_ShaderInformation.PushConstantInformation.Offset);
		}
	}

	void ShaderCode::ReflectResource(const spirv_cross::Compiler& compiler, const spirv_cross::Resource& resource)
	{
		const spirv_cross::SPIRType& bufferType = compiler.get_type(resource.base_type_id);
		uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
		uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
		
		ONYX_CORE_TRACE("  {0}", resource.name);
		ONYX_CORE_TRACE("    Size = {0}", bufferSize);
		ONYX_CORE_TRACE("    Binding = {0}", binding);
		ONYX_CORE_TRACE("    Members:");

		for (int i = 0; i < bufferType.member_types.size(); i++)
		{
			ReflectMember(compiler, resource, bufferType, i);
		}
	}

	void ShaderCode::ReflectMember(const spirv_cross::Compiler& compiler, const spirv_cross::Resource& resource,
		const spirv_cross::SPIRType& bufferType, int index)
	{
		auto type = compiler.get_type(bufferType.member_types[index]);
		std::string name = compiler.get_member_name(resource.base_type_id, index);
		ONYX_CORE_TRACE("      {0} {1}", TypeToString(type), name);
	}

	std::string ShaderCode::TypeToString(spirv_cross::SPIRType type)
	{
		switch (type.op) {
			case spv::OpTypeVoid: return "void";
			case spv::OpTypeBool: return "bool";
			case spv::OpTypeInt: return "int";
			case spv::OpTypeFloat: return "float";
			case spv::OpTypeVector: 
			{
				uint32_t component_count = type.vecsize;
				return "vec" + std::to_string(component_count);
			}
			case spv::OpTypeMatrix: 
			{
				uint32_t column_count = type.vecsize;
				return "mat" + std::to_string(column_count);
			}
			case spv::OpTypeSampler: return "sampler";
			case spv::OpTypeImage: return "image";
			case spv::OpTypeSampledImage: return "sampled_image";
			case spv::OpTypeArray: return "array";
			case spv::OpTypeRuntimeArray: return "runtime_array";
			case spv::OpTypeStruct: return "struct"; // Handle separately if needed
			case spv::OpTypePointer: return "pointer"; // Handle separately if needed
			default: return "unknown";
		}
	}
}